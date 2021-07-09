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
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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
Window::Window(int width, int height, const char* name)
	:
	m_Width(width),
	m_Height(height),
	m_IsCursorEnabled(true),
	m_Keyboard(std::make_unique<Keyboard>()),
	m_Mouse(std::make_unique<Mouse>())
{
	// Calculate window size based on desired client region
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WND_LAST_EXCEPT();
	}

	// Create window and get its handle
	m_Handle = CreateWindow(
		WndClass::GetName(),
		name,
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(wr.right - wr.left),
		(wr.bottom - wr.top),
		nullptr,
		nullptr,
		WndClass::GetInstance(),
		this										// Pointer to the window instance to work along with HandleMessageSetup function. THIS IS THE #SURPRIIISOOOOOOO
	);

	if (m_Handle == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	ShowWindow(m_Handle, SW_SHOWDEFAULT);


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
	DestroyWindow(m_Handle);
}

void Window::SetTitle(const char* title)
{
	if (SetWindowText(m_Handle, title) == 0)
	{
		throw WND_LAST_EXCEPT();
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
	GetClientRect(m_Handle, &r);
	MapWindowPoints(m_Handle, nullptr, reinterpret_cast<POINT*>(&r), 2);
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

void Window::OnClose_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	PostQuitMessage(0);
}

void Window::OnClosing_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

}

void Window::OnClosed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

}

void Window::OnFocusEnter_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

}

void Window::OnFocusLeave_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_Keyboard->ClearState();
}

void Window::OnMouseMove_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);

	// Cursorless exclusive gets first dibs
	if (!m_IsCursorEnabled)
	{
		if (!m_Mouse->IsInWindow())
		{
			SetCapture(m_Handle);
			m_Mouse->OnMouseEnter();
			HideCursor();
		}

		return;
	}

	// Inside client region -> log move, and log enter + capture mouse (if not previously in window)
	if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
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
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		// Exit message to be handled in application class
	case WM_CLOSE: OnClose_Impl(hWnd, msg, wParam, lParam); OnClose(); return 0;
	case WM_DESTROY: OnClosing_Impl(hWnd, msg, wParam, lParam); OnClosing(); break;
	case WM_NCDESTROY: OnClosed_Impl(hWnd, msg, wParam, lParam); OnClosed(); break;
	case WM_SETFOCUS: OnFocusEnter_Impl(hWnd, msg, wParam, lParam); OnFocusEnter(); break;
		// Clear key state when window loses focus to prevent input getting "stuck" 
	case WM_KILLFOCUS: OnFocusLeave_Impl(hWnd, msg, wParam, lParam); OnFocusLeave(); break;

	case WM_ACTIVATE:
	{
		// Confine/Free cursor on window to foreground/background if cursor disabled
		if (m_IsCursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				EncloseCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}

		break;
	}

	/************************ KEYBOARD MESSAGES ************************/

	case WM_KEYDOWN:
		// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYDOWN:
	{
		// Filter AutoRepeat key events
		if (!(lParam & 0x40000000) || m_Keyboard->IsAutoRepeatEnabled())
		{
			m_Keyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		}

		break;
	}

	case WM_KEYUP:
		// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_SYSKEYUP:
	{
		m_Keyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}

	case WM_CHAR:
	{
		m_Keyboard->OnChar(static_cast<unsigned char>(wParam));
		break;
	}

	/********************** END KEYBOARD MESSAGES **********************/

	/********************** MOUSE MESSAGES **********************/
	case WM_MOUSEMOVE:
	{
		OnMouseMove_Impl(hWnd, msg, wParam, lParam);
		OnMouseMove();
		break;
	}

	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(m_Handle);
		if (!m_IsCursorEnabled)
		{
			EncloseCursor();
			HideCursor();
		}

		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse->OnLeftPressed(pt.x, pt.y);

		break;
	}

	case WM_RBUTTONDOWN:
	{
		{
			const POINTS pt = MAKEPOINTS(lParam);
			m_Mouse->OnRightPressed(pt.x, pt.y);
		}

		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse->OnLeftReleased(pt.x, pt.y);

		break;
	}

	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_Mouse->OnRightReleased(pt.x, pt.y);

		break;
	}

	case WM_MOUSEWHEEL:
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

		break;
	}

	/******************** END MOUSE MESSAGES ********************/

	/******************** RAW MOUSE MESSAGES ********************/

	case WM_INPUT:
	{
		if (!m_Mouse->IsRawEnabled())
		{
			break;
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
			break;
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
			break;
		}

		// Process the raw input data
		const auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			m_Mouse->OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}


		break;
	}

	/****************** END RAW MOUSE MESSAGES ******************/

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);;
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

	return pMessageBuffer;
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