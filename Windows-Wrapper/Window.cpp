#include "Window.h"

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
	wc.style = CS_HREDRAW | CS_VREDRAW;
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
	Control(name, width, height, 0, 0),
	m_IsCursorEnabled(true),
	m_Keyboard(std::make_unique<Keyboard>()),
	m_Mouse(std::make_unique<Mouse>())
{
	// Calculate window size based on desired client region
	m_Rectangle.left = 100;
	m_Rectangle.right = width + m_Rectangle.left;
	m_Rectangle.top = 100;
	m_Rectangle.bottom = height + m_Rectangle.top;
	if (AdjustWindowRect(&m_Rectangle, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WND_LAST_EXCEPT();
	}
	
	// Create window and get its handle
	Handle = CreateWindow(
		WndClass::GetName(),																// Class name
		name.c_str(),																				// Window title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,	// Style values
		CW_USEDEFAULT,																		// X position
		CW_USEDEFAULT,																		// Y position
		(m_Rectangle.right - m_Rectangle.left),																// Width
		(m_Rectangle.bottom - m_Rectangle.top),																// Height
		nullptr,																			// Parent handle
		nullptr,																			// Menu handle
		WndClass::GetInstance(),															// Module instance handle
		this																				// Pointer to the window instance to work along with HandleMessageSetup function. THIS IS THE #SURPRIIISOOOOOOO
	);

	if (Handle.IsNull())
	{
		throw WND_LAST_EXCEPT();
	}

	ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);

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
		throw WND_LAST_EXCEPT();
	}
}

Window::~Window()
{
	DestroyWindow(static_cast<HWND>(Handle.ToPointer()));
}

EventDispatcher& Window::GetEventDispatcher() const noexcept
{
	return *m_Events;
}

void Window::SetText(const std::string& title)
{
	if (SetWindowText(static_cast<HWND>(Handle.ToPointer()), title.c_str()) == 0)
	{
		throw WND_LAST_EXCEPT();
	}

	Text = title;
	RedrawWindow(static_cast<HWND>(Handle.ToPointer()), NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
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

const std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Check for quit message because PeekMessage doesn't signal it via return val
		if (msg.message == WM_QUIT)
		{
			return static_cast<int>(msg.wParam);
		}

		// Translate and dispatch messages
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return{};
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

// This function is responsible to change the default HandleMessage function pointer to the custom one.
// It works as a Window class wrapper, allowing multiple Window instances to be easily instantiated.
// YOU ARE NOT SUPPOSED TO UNDERSTAND THIS AS NEITHER DO I UNDERSTAND :'(
// LOTS OF THANKS TO STACKOVERFLOW :D
LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data do store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message function to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageForwarder));

		// Forward message to window class member function
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept
{
	// Retrieve pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->HandleMessage(hWnd, msg, lParam, wParam);
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
	//	for(const auto& m : m_Menu)
	//		if(m_Menu-> == wParam) // Localize the right action
	// 			m.func_pointer	// Invoke the function pointer
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

	OnMouseLeftDown();
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
	case WM_COMMAND: OnCommand_Impl(hWnd, msg, wParam, lParam);
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
		//int width = LOWORD(lParam);
		//int height = HIWORD(lParam);

		//// Get the window and client dimensions
		//RECT window;
		//RECT window2;

		//GetWindowRect(hWnd, &window);
		//GetClientRect(hWnd, &window2);

		//if (width < (height * m_Width) / m_Height)
		//{
		//	// Calculate desired window width and height
		//	int border = (window.right - window.left) -
		//		window2.right;
		//	int header = (window.bottom - window.top) -
		//		window2.bottom;
		//	width = ((height * m_Width) / m_Height) + border;
		//	height = height + header; // + toolbar.rect.bottom

		//	// Set new dimensions
		//	SetWindowPos(hWnd, NULL, 0, 0,
		//		width, height,
		//		SWP_NOMOVE | SWP_NOZORDER);
		//}

		//break;
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

// Exceptions
const std::string& Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMessageBuffer = nullptr;

	// Windows will allocate memory for error string and make our pointer point to it
	const DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer),
		0,
		nullptr);

	// String length as 0 indicates a failure
	if (nMessageLength == 0)
	{
		return "Unidentified error code";
	}

	// Copy error string from windows allocated buffer to string
	std::string errorString = pMessageBuffer;

	// Free windows buffer
	LocalFree(pMessageBuffer);

	return errorString;
}

Window::HRException::HRException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{ }

const char* Window::HRException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
		<< "Description: " << GetErrorDescription() << std::endl
		<< GetErrorSpot();
	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const char* Window::HRException::GetType() const noexcept
{
	return "Window Exception";
}

HRESULT Window::HRException::GetErrorCode() const noexcept
{
	return hr;
}

const std::string& Window::HRException::GetErrorDescription() const noexcept
{
	return WindowException::TranslateErrorCode(hr);
}

MenuBar& Window::CreateMenuBar(const std::string& text) noexcept
{
	if (m_Menu == nullptr)
	{
		m_Menu = std::make_unique<MenuBar>(new MenuBar(this, text));
	}

	return *m_Menu;
}

void Window::DestroyMenu()
{
	m_Menu.reset();
}

void Window::SetMenuHeader(const std::string& text)
{
	m_Menu->SetText(text);
}