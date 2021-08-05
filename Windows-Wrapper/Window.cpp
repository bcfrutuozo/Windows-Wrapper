#include "Window.h"
#include "Event.h"
#include "OnClosedEventHandler.h"
#include "OnClosingEventHandler.h"

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
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
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

Window::Window(const std::string& name, int width, int height)
	:
	IWinControl(nullptr, name, width, height, 0, 0),
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

void Window::OnClosingSet(const std::function<void(Control* const c, OnClosingEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<OnClosingEventHandler>("OnClosing", callback));
}

void Window::OnShownSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnShown", callback));
}

void Window::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
}

void Window::OnClosedSet(const std::function<void(Control* const c, OnClosedEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<OnClosedEventHandler>("OnClosing", callback));
}

void Window::Initialize() noexcept
{
	// Calculate window size based on desired client region
	RECT r;
	r.left = 100;
	r.right = Size.Width + r.left;
	r.top = 100;
	r.bottom = Size.Height + r.top;
	if (AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE, FALSE) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	// Create window and get its handle
	Handle = CreateWindow(
		WndClass::GetName(),																// Class name
		Text.c_str(),																		// Window title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,	// Style values
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

	// Force window redraw to set Background color
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
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

TextBox& Window::AddTextBox(const std::string& name, int width, int height, int x, int y) noexcept
{
	return Create<TextBox>(this, name, width, height, x, y);
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
	if (m_Mouse != nullptr)
	{
		m_IsCursorEnabled = true;
		ShowCursor();
		FreeCursor();
	}
}

void Window::DisableCursor() noexcept
{
	if (m_Mouse != nullptr)
	{
		m_IsCursorEnabled = false;
		HideCursor();
		EncloseCursor();
	}
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
		Events.Dispatch("OnActivate");
	}
	else
	{
		Events.Dispatch("OnDeactivate");
	}
}

void Window::OnCommand_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Menu
	auto cmd = HIWORD(wParam);

	if (cmd == 0)
	{
		if (const auto& c = GetById(static_cast<unsigned int>(wParam)))
		{
			// Dispatch both OnClick for common task and OnMouseClick which receives the cursor information
			c->Dispatch("OnClick", new EventArgs());
			c->Dispatch("OnMouseClick", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));

			// Force the update of the controls
			c->Dispatch("OnInternalUpdate", new EventArgs());
		}
	}

	// Accelerators
	else if (cmd == 1)
	{

	}

	// Controls
	else
	{
		if (const auto& c = GetByHandle((HWND)(lParam)))
		{
			// TextBox events
			switch (cmd)
			{

			case BCN_DROPDOWN:break;
			case BCN_HOTITEMCHANGE:break;
			case BN_CLICKED: c->Dispatch("OnClick", new EventArgs()); break;
			case BN_DBLCLK:break;
			case BN_DISABLE:break;
			case BN_KILLFOCUS:break;
			case BN_PAINT:break;
			case BN_PUSHED:break;
			case BN_SETFOCUS:break;
			case BN_UNPUSHED:break;

			case EN_SETFOCUS: c->Dispatch("OnGotFocus", new EventArgs()); break;
			case EN_KILLFOCUS: c->Dispatch("OnLostFocus", new EventArgs()); break;
			case EN_CHANGE: OutputDebugString("a"); break;
			case EN_UPDATE: OutputDebugString("a"); break;
			case EN_ERRSPACE: OutputDebugString("a"); break;
			case EN_MAXTEXT: OutputDebugString("a"); break;
			case EN_HSCROLL: OutputDebugString("a"); break;
			case EN_VSCROLL: OutputDebugString("a"); break;
			}
		}
	}
}

void Window::OnClose_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	PostQuitMessage(0);
}

void Window::OnCreate_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{


	Dispatch("OnCreate");
}

void Window::OnEraseBackground_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	HDC hdc = (HDC)(wParam);
	RECT rc; GetClientRect(static_cast<HWND>(Handle.ToPointer()), &rc);
	HBRUSH brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdc, &rc, brush);

	Dispatch("OnErase");
}

void Window::OnDestroy_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	bool cancel = false;
	Dispatch("OnClosing", new OnClosingEventArgs(CloseReason::UserClosing, cancel));

	if (cancel)
	{
		// TODO: CANCEL FORM CLOSING
	}
	else
	{

	}
}

void Window::OnClosed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Dispatch("OnClosing", new OnClosedEventArgs(CloseReason::UserClosing));
}

void Window::OnFocusEnter_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Dispatch("OnGotFocus", new EventArgs());
}

void Window::OnFocusLeave_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Dispatch("OnLostFocus", new EventArgs());

	// Clear key state when window loses focus to prevent input getting "stuck"
	if (m_Keyboard != nullptr)
	{
		m_Keyboard->ClearState();
	}
}

void Window::OnKeyDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Filter AutoRepeat key events
	if (m_Keyboard != nullptr)
	{
		if (!(lParam & 0x40000000) || m_Keyboard->IsAutoRepeatEnabled())
		{
			m_Keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	}

	Dispatch("OnKeyDown", new KeyEventArgs(static_cast<Keys>(wParam)));
}

void Window::OnKeyPressed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (m_Keyboard != nullptr)
	{
		m_Keyboard->OnChar(static_cast<unsigned char>(wParam));
	}

	Dispatch("OnKeyPress", new KeyPressEventArgs(static_cast<unsigned char>(wParam)));
}

void Window::OnKeyUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (m_Keyboard != nullptr)
	{
		m_Keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
	}

	Dispatch("OnKeyUp", new KeyEventArgs(static_cast<Keys>(wParam)));
}

void Window::OnMouseMove_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	// COMMENTED LOGIC USED FOR REALTIME APPLICATION. GOING TO FIGURE OUT HOW TO SEPARATE IN TWO WINDOWS MAYBE?
	// Cursorless exclusive gets first dibs
	//if (!m_IsCursorEnabled)
	//{
	//	if (!m_Mouse->IsInWindow())
	//	{
	//		SetCapture(static_cast<HWND>(Handle.ToPointer()));
	//		m_Mouse->OnMouseEnter();
	//		HideCursor();
	//	}
	//
	//	return;
	//}

	// Inside client region -> log move, and log enter + capture mouse (if not previously in window)
	//if (pt.x >= 0 && pt.x < Size.Width && pt.y >= 0 && pt.y < Size.Height)
	//{
	//	m_Mouse->OnMouseMove(pt.x, pt.y);
	//	if (!m_Mouse->IsInWindow())
	//	{
	//		SetCapture(hWnd);
	//		m_Mouse->OnMouseEnter();
	//	}
	//}
	//
	//// Out of client region -> log move / maintain capture if button down
	//else
	//{
	//	if (wParam & (MK_LBUTTON | MK_RBUTTON))
	//	{
	//		m_Mouse->OnMouseMove(pt.x, pt.y);
	//	}
	//	// Button up -> release capture / log event for leaving
	//	else
	//	{
	//		ReleaseCapture();
	//		m_Mouse->OnMouseLeave();
	//	}
	//}

	// Inside client region -> log move, and log enter + capture mouse (if not previously in window)
	if (pt.x >= 0 && pt.x < Size.Width && pt.y >= 0 && pt.y < Size.Height)
	{
		m_Mouse->OnMouseMove(pt.x, pt.y);
		if (!m_Mouse->IsInWindow())
		{
			m_Mouse->OnMouseEnter();
			Dispatch("OnMouseEnter", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
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
			m_Mouse->OnMouseLeave();
			Dispatch("OnMouseLeave", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
		}
	}
}

void Window::OnMouseLeftDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//SetForegroundWindow(static_cast<HWND>(Handle.ToPointer()));

	if (m_Mouse != nullptr)
	{
		if (!m_IsCursorEnabled)
		{
			EncloseCursor();
			HideCursor();
		}

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse->OnLeftPressed(pt.x, pt.y);
	}

	Dispatch("OnMouseLeftDown", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseLeftUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	if (m_Mouse != nullptr)
	{
		m_Mouse->OnLeftReleased(pt.x, pt.y);
	}

	// OnClick event is always executed before the MouseUp event with both buttons
	Dispatch("OnClick", new EventArgs());

	Dispatch("OnMouseLeftUp", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseRightDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	if (m_Mouse != nullptr)
	{
		m_Mouse->OnRightPressed(pt.x, pt.y);
	}

	Dispatch("OnMouseRightDown", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseRightUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	if (m_Mouse != nullptr)
	{
		m_Mouse->OnRightReleased(pt.x, pt.y);
	}

	// OnClick event is always executed before the MouseUp event with both buttons
	Dispatch("OnClick", new EventArgs());

	Dispatch("OnMouseRightUp", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseLeftDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Dispatch("OnMouseLeftDoubleClick", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseRightDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Dispatch("OnMouseRightDoubleClick", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

void Window::OnMouseWheel_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	{
		if (m_Mouse != nullptr)
		{
			m_Mouse->OnWheelUp(pt.x, pt.y);
		}
	}
	else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
	{
		if (m_Mouse != nullptr)
		{
			m_Mouse->OnWheelDown(pt.x, pt.y);
		}
	}

	Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
}

int Window::OnNotify_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	LPNMHDR some_item = (LPNMHDR)lParam;
	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

	SetBkMode(item->hdc, OPAQUE);

	auto control = GetByHandle(some_item->hwndFrom);

	switch (some_item->code)
	{
	case BCN_HOTITEMCHANGE:
	{
		NMBCHOTITEM* hot_item = reinterpret_cast<NMBCHOTITEM*>(lParam);

		// Handle to the button
		HWND button_handle = some_item->hwndFrom;

		// ID of the button, if you're using resources
		UINT_PTR button_id = some_item->idFrom;

		// You can check if the mouse is entering or leaving the hover area
		bool entering = hot_item->dwFlags & HICF_ENTERING;

		if (entering)
		{
			control->Dispatch("OnMouseEnter", new EventArgs());
		}
		else
		{
			control->Dispatch("OnMouseLeave", new EventArgs());
		}
	}
	}

	// Cancel design if button style is the default one
	if (control->m_BackgroundColor == Color::Default() && control->m_ForeColor == Color::Black())
	{
		return CDRF_DODEFAULT;
	}

	// Select color when the button is selected
	m_Brush = CreateGradientBrush(control->m_BackgroundColor, control->m_BackgroundColor, item);

	//Create pen for button border
	HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

	//Select our brush into hDC
	HGDIOBJ old_pen = SelectObject(item->hdc, pen);
	HGDIOBJ old_brush = SelectObject(item->hdc, m_Brush);

	// GOING TO THINK A WAY OF IMPLEMENT NORMAL AND ROUNDED BORDER BUTTON

	//If you want rounded button, then use this, otherwise use FillRect().
	//RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 10, 10);
	Rectangle(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom);
	//FillRect(item->hdc, &item->rc, m_Brush);

	//Clean up
	SelectObject(item->hdc, old_pen);
	SelectObject(item->hdc, old_brush);
	DeleteObject(pen);

	SetBkMode(item->hdc, TRANSPARENT);
	SetTextColor(item->hdc, RGB(control->m_ForeColor.GetR(), control->m_ForeColor.GetB(), control->m_ForeColor.GetB()));
	DrawText(item->hdc, control->GetText().c_str(), -1, &item->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (item->uItemState & CDIS_SELECTED)
	{
		return CDRF_DODEFAULT;
	}
	else
	{
		return CDRF_SKIPDEFAULT;
	}

	return 0;
}

void Window::OnRawInput_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (m_Mouse == nullptr && !m_Mouse->IsRawEnabled())
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
		if (m_Mouse != nullptr)
		{
			m_Mouse->OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
	}
}

void Window::OnSetCursor_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (const auto& c = GetByHandle((HWND)(wParam)))
	{
		// Dispatch both OnClick for common task and OnMouseClick which receives the cursor information
		c->Dispatch("OnMouseMove", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));
	}
}

void Window::OnShowWindow_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (wParam == TRUE)
	{
		Dispatch("OnShown", new EventArgs());
	}
	else
	{
		Dispatch("OnHide", new EventArgs());
	}
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//OutputDebugString(Mapper(msg, wParam, lParam).c_str());

	switch (msg)
	{
	case WM_SETCURSOR: OnSetCursor_Impl(hWnd, msg, wParam, lParam); break;
	case WM_CREATE: OnCreate_Impl(hWnd, msg, wParam, lParam); break;
	case WM_ERASEBKGND: OnEraseBackground_Impl(hWnd, msg, wParam, lParam); break;
	case WM_COMMAND: OnCommand_Impl(hWnd, msg, wParam, lParam); break;
	case WM_CLOSE: OnClose_Impl(hWnd, msg, wParam, lParam); return 0;		// Exit message to be handled in application class
	case WM_DESTROY: OnDestroy_Impl(hWnd, msg, wParam, lParam); break;
	case WM_NCDESTROY: OnClosed_Impl(hWnd, msg, wParam, lParam); break;
	case WM_SETFOCUS: OnFocusEnter_Impl(hWnd, msg, wParam, lParam); break;
	case WM_KILLFOCUS: OnFocusLeave_Impl(hWnd, msg, wParam, lParam); break;
	case WM_ACTIVATE: OnActivate_Impl(hWnd, msg, wParam, lParam); break;
	case WM_NOTIFY: return OnNotify_Impl(hWnd, msg, wParam, lParam);

		// Set the button border color inside window control to match the same color as the BackgroundColor
	case WM_CTLCOLORBTN:
	{
		return (LRESULT)CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	}
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
	case WM_SHOWWINDOW: OnShowWindow_Impl(hWnd, msg, wParam, lParam); break;
		break;
		/******************** END MOUSE MESSAGES ********************/
		/******************** RAW MOUSE MESSAGES ********************/
	case WM_INPUT: OnRawInput_Impl(hWnd, msg, wParam, lParam); break;
		/****************** END RAW MOUSE MESSAGES ******************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}