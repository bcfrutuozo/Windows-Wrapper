#include "Window.h"
#include "Event.h"
#include "OnClosedEventHandler.h"
#include "OnClosingEventHandler.h"

#include <sstream>

// Singleton WndClass
Window::WindowClass Window::WindowClass::m_WindowClass;

// Window class declarations
Window::WindowClass::WindowClass() noexcept
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

Window::WindowClass::~WindowClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return m_WindowClass.m_Instance;
}

Window::Window(const std::string& name, int width, int height)
	:
	WinControl(nullptr, name, width, height, 0, 0),
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
		WindowClass::GetName(),																// Class name
		Text.c_str(),																		// Window title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,	// Style values
		CW_USEDEFAULT,																		// X position
		CW_USEDEFAULT,																		// Y position
		(r.right - r.left),																	// Width
		(r.bottom - r.top),																	// Height
		nullptr,																			// Parent handle
		nullptr,																			// Menu handle
		WindowClass::GetInstance(),															// Module instance handle
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

void Window::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept
{
	if (const auto& c = GetById(id))
	{
		// Dispatch both OnClick for common task and OnMouseClick which receives the cursor information
		c->Dispatch("OnClick", new EventArgs());
		c->Dispatch("OnMouseClick", new MouseEventArgs(MouseButtons::Left, 1, 0, 0, 0));

		// Force the update of the controls
		c->Dispatch("OnInternalUpdate", new EventArgs());
	}
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