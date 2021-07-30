#include "Window.h"
#include "Event.h"

#include <sstream>

// Singleton WndClass
Window::WndClass Window::WndClass::m_WndClass;

// Window class declarations
Window::WndClass::WndClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	//wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	//wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WndClass::~WndClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Window::WndClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Window::WndClass::GetInstance() noexcept
{
	return m_WndClass.m_Instance;
}

// Window
Window::Window(const std::string& name, int width, int height)
	:
	Control(nullptr, name, width, height, 0, 0),
	m_IsCursorEnabled(true),
	m_Keyboard(std::make_unique<Keyboard>()),
	m_Mouse(std::make_unique<Mouse>()),
	m_IsMenuStripEnabled(false)
{
	Initialize();
}

void Window::Hide()
{
	if (IsShown())
	{
		IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void Window::Show()
{
	if (!IsShown())
	{
		IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}

void Window::Initialize() noexcept
{
	// Calculate window size based on desired client region
	RECT r;
	r.left = 100;
	r.right = Size.Width + r.left;
	r.top = 100;
	r.bottom = Size.Height + r.top;
	if (AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	// Create window and get its handle
	Handle = CreateWindow(
		WndClass::GetName(),																// Class name
		Text.c_str(),																		// Window title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,	// Style values
		CW_USEDEFAULT,																		// X position
		CW_USEDEFAULT,																		// Y position
		(r.right - r.left),																	// Width
		(r.bottom - r.top),																	// Height
		nullptr,																			// Parent handle
		nullptr,																			// Menu handle
		WndClass::GetInstance(),															// Module instance handle
		this																				// Pointer to the window instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// TODO: THIS IS WHERE THE GRAPHICS DEVICE, CONTEXT, RENDER TARGET, DEPTHSTENCIL (MAYBE) WILL BE INSTANTIATED.
	// YES: THIS IS GONNA TAKE SOME TIME DO DESIGN
	// AND YES AS WELL: WILL BE PROBABLY POORLY DONE :( 

	// Register mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // Mouse page
	rid.usUsage = 0x02; // Mouse page
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		throw CTL_LAST_EXCEPT();
	}
	// Still don't know how I'll implement child windows. So this is just to not let an empty ugly function
	//SetParent(static_cast<HWND>(Handle.ToPointer()), static_cast<HWND>(Parent->Handle.ToPointer()));
}

Window::~Window()
{
	DestroyWindow(static_cast<HWND>(Handle.ToPointer()));
}

void Window::ClearMenuStrip() noexcept
{
	SetMenu(static_cast<HWND>(Handle.ToPointer()), NULL);

	for (auto& c : Controls)
	{
		if (c->GetType() == typeid(MenuStrip))
		{
			c->Delete();
			const auto it = std::find(Controls.begin(), Controls.end(), c);
			Controls.erase(it);

			// Break processing to avoid exception.
			// We are removing an element inside a for loop which will break the for iterator.
			// And as we have just one MenuStrip per Window. It's a valid assumption.
			break;
		}
	}
}

void Window::UpdateMenuStrip() noexcept
{
	for (auto& c : Controls)
	{
		if (c->GetType() == typeid(MenuStrip))
		{
			SetMenu(static_cast<HWND>(Handle.ToPointer()), static_cast<HMENU>(c->Handle.ToPointer()));
			m_IsMenuStripEnabled = true;
		}
	}
}

MenuStrip& Window::GetMenuStrip() noexcept
{
	for (auto& c : Controls)
	{
		if (c->GetType() == typeid(MenuStrip))
		{
			return dynamic_cast<MenuStrip&>(*c);
		}
	}

	return Create<MenuStrip>(this);
}

Button& Window::AddButton(const std::string& name, int width, int height, int x, int y) noexcept
{
	return Create<Button>(this, name, width, height, x, y);
}

void Window::SetText(const std::string& title)
{
	if (IsShown())
	{
		if (SetWindowText(static_cast<HWND>(Handle.ToPointer()), title.c_str()) == 0)
		{
			throw CTL_LAST_EXCEPT();
		}

		Text = title;
		RedrawWindow(static_cast<HWND>(Handle.ToPointer()), NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
	}
}

void Window::EnableCursor() noexcept
{
	m_IsCursorEnabled = true;
	ShowCursor();
	FreeCursor();
}

void Window::DisableCursor() noexcept
{
	m_IsCursorEnabled = false;
	HideCursor();
	EncloseCursor();
}

void Window::EncloseCursor() const noexcept
{
	RECT r;
	GetClientRect(static_cast<HWND>(Handle.ToPointer()), &r);
	MapWindowPoints(static_cast<HWND>(Handle.ToPointer()), nullptr, reinterpret_cast<POINT*>(&r), 2);
	ClipCursor(&r);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

bool Window::IsCursorEnabled() const noexcept
{
	return m_IsCursorEnabled;
}

Color Window::GetForeColor() noexcept
{
	return m_ForeColor;
}

void Window::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
}

Keyboard& Window::GetKeyboard() noexcept
{
	if (!m_Keyboard)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Keyboard;
}


Mouse& Window::GetMouse() noexcept
{
	if (!m_Mouse)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Mouse;
}

// Event handling implementation
void Window::OnActivate_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (wParam & WA_ACTIVE)
	{
		OnActivation();
	}
	else
	{
		OnDeactivation();
	}
}

void Window::OnCommand_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	for (const auto& item : Controls)
	{
		// Get the MenuStrip to search for the right item
		auto menu = dynamic_cast<MenuStrip*>(item.get());
		if (menu != nullptr)
		{
			auto entry = menu->GetById(static_cast<unsigned int>(wParam));
			if (entry != nullptr)
			{
				// Dispatch both OnClick for common task and OnMouseClick which receives the cursor information
				entry->Dispatch("OnClick", new EventArgs());
				entry->Dispatch("OnMouseClick", new MouseEventArgs(m_Mouse.get()));

				// Force the update of the controls
				entry->Dispatch("OnInternalUpdate", new EventArgs());
			}
		}
	}
}

void Window::OnClose_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	PostQuitMessage(0);
	OnClose();
}

void Window::OnClosing_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OnClosing();
}

void Window::OnClosed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OnClosed();
}

void Window::OnFocusEnter_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OnFocusEnter();
}

void Window::OnFocusLeave_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Clear key state when window loses focus to prevent input getting "stuck" 
	m_Keyboard->ClearState();
	OnFocusLeave();
}

void Window::OnKeyDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Filter AutoRepeat key events
	if (!(lParam & 0x40000000) || m_Keyboard->IsAutoRepeatEnabled())
	{
		m_Keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
	}

	OnKeyDown();
}

void Window::OnKeyPressed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_Keyboard->OnChar(static_cast<unsigned char>(wParam));
	OnKeyPressed();
}

void Window::OnKeyUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_Keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
	OnKeyUp();
}

void Window::OnMouseMove_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	// Cursorless exclusive gets first dibs
	if (!m_IsCursorEnabled)
	{
		if (!m_Mouse->IsInWindow())
		{
			SetCapture(static_cast<HWND>(Handle.ToPointer()));
			m_Mouse->OnMouseEnter();
			HideCursor();
		}

		return;
	}

	// Inside client region -> log move, and log enter + capture mouse (if not previously in window)
	if (pt.x >= 0 && pt.x < Size.Width && pt.y >= 0 && pt.y < Size.Height)
	{
		m_Mouse->OnMouseMove(pt.x, pt.y);
		if (!m_Mouse->IsInWindow())
		{
			SetCapture(hWnd);
			m_Mouse->OnMouseEnter();
		}
	}
	// Out of client region -> log move / maintain capture if button down
	else
	{
		if (wParam & (MK_LBUTTON | MK_RBUTTON))
		{
			m_Mouse->OnMouseMove(pt.x, pt.y);
		}
		// Button up -> release capture / log event for leaving
		else
		{
			ReleaseCapture();
			m_Mouse->OnMouseLeave();
		}
	}

	OnMouseMove();
}

void Window::OnMouseLeftDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	SetForegroundWindow(static_cast<HWND>(Handle.ToPointer()));

	if (!m_IsCursorEnabled)
	{
		EncloseCursor();
		HideCursor();
	}

	const POINTS pt = MAKEPOINTS(lParam);
	m_Mouse->OnLeftPressed(pt.x, pt.y);
	Dispatch("OnClick", new MouseEventArgs(m_Mouse.get()));
}

void Window::OnMouseLeftUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_Mouse->OnLeftReleased(pt.x, pt.y);

	OnMouseLeftUp();
}

void Window::OnMouseRightDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_Mouse->OnRightPressed(pt.x, pt.y);

	OnMouseRightDown();
}

void Window::OnMouseRightUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_Mouse->OnRightReleased(pt.x, pt.y);

	OnMouseRightUp();
}

void Window::OnMouseLeftDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OnMouseLeftDoubleClick();
}

void Window::OnMouseRightDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OnMouseRightDoubleClick();
}

void Window::OnMouseWheel_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	{
		m_Mouse->OnWheelUp(pt.x, pt.y);
	}
	else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
	{
		m_Mouse->OnWheelDown(pt.x, pt.y);
	}

	OnMouseWheel();
}

void Window::OnNotify_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

	OnNotify();
}

void Window::OnRawInput_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (!m_Mouse->IsRawEnabled())
	{
		return;
	}

	UINT size = 0;
	// First get the size of the input data
	if (GetRawInputData(
		reinterpret_cast<HRAWINPUT>(lParam),
		RID_INPUT,
		nullptr,
		&size,
		sizeof(RAWINPUTHEADER)) == -1)
	{
		// Ignore messaging errors
		return;
	}
	m_RawBuffer.resize(size);

	// Read in the input data
	if (GetRawInputData(
		reinterpret_cast<HRAWINPUT>(lParam),
		RID_INPUT,
		m_RawBuffer.data(),
		&size,
		sizeof(RAWINPUTHEADER)) != size)
	{
		// Ignore messaging errors
		return;
	}

	// Process the raw input data
	const auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
	if (ri.header.dwType == RIM_TYPEMOUSE &&
		(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
	{
		m_Mouse->OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
	}
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_COMMAND: OnCommand_Impl(hWnd, msg, wParam, lParam); break;
	case WM_CLOSE: OnClose_Impl(hWnd, msg, wParam, lParam); return 0;		// Exit message to be handled in application class
	case WM_DESTROY: OnClosing_Impl(hWnd, msg, wParam, lParam); break;
	case WM_NCDESTROY: OnClosed_Impl(hWnd, msg, wParam, lParam); break;
	case WM_SETFOCUS: OnFocusEnter_Impl(hWnd, msg, wParam, lParam); break;
	case WM_KILLFOCUS: OnFocusLeave_Impl(hWnd, msg, wParam, lParam); break;
	case WM_ACTIVATE: OnActivate_Impl(hWnd, msg, wParam, lParam); break;
		/******************** KEYBOARD MESSAGES *********************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: OnKeyDown_Impl(hWnd, msg, wParam, lParam); break;	// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_KEYUP:
	case WM_SYSKEYUP: OnKeyUp_Impl(hWnd, msg, wParam, lParam); break;		// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_CHAR: OnKeyPressed_Impl(hWnd, msg, wParam, lParam); break;
		/******************* END KEYBOARD MESSAGES ******************/
		/********************** MOUSE MESSAGES **********************/
	case WM_MOUSEMOVE: 	OnMouseMove_Impl(hWnd, msg, wParam, lParam); break;
	case WM_LBUTTONDOWN: OnMouseLeftDown_Impl(hWnd, msg, wParam, lParam); break;
	case WM_LBUTTONUP: OnMouseLeftUp_Impl(hWnd, msg, wParam, lParam); break;
	case WM_RBUTTONDOWN: OnMouseRightDown_Impl(hWnd, msg, wParam, lParam); break;
	case WM_RBUTTONUP: OnMouseRightUp_Impl(hWnd, msg, wParam, lParam); break;
	case WM_LBUTTONDBLCLK: OnMouseLeftDoubleClick_Impl(hWnd, msg, wParam, lParam); break;
	case WM_RBUTTONDBLCLK: OnMouseRightDoubleClick_Impl(hWnd, msg, wParam, lParam); break;
	case WM_MOUSEWHEEL: OnMouseWheel_Impl(hWnd, msg, wParam, lParam); break;
	case WM_NOTIFY: OnNotify_Impl(hWnd, msg, wParam, lParam); break;
		/******************** END MOUSE MESSAGES ********************/
		/******************** RAW MOUSE MESSAGES ********************/
	case WM_INPUT: OnRawInput_Impl(hWnd, msg, wParam, lParam); break;
		/****************** END RAW MOUSE MESSAGES ******************/
	case WM_SIZE:
	{
		break;
	}
	case WM_SIZING:
	{
		PRECT r = (PRECT)lParam;

		HBRUSH brush = CreateSolidBrush(RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		SetClassLongPtr(static_cast<HWND>(Handle.ToPointer()), GCLP_HBRBACKGROUND, (LONG_PTR)brush);

		break;
	}
	case WM_PAINT:
	{
		PRECT r = (PRECT)lParam;

		HBRUSH brush = CreateSolidBrush(RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		SetClassLongPtr(static_cast<HWND>(Handle.ToPointer()), GCLP_HBRBACKGROUND, (LONG_PTR)brush);

		break;
	}

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}