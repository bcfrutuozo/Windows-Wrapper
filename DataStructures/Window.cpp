#include "Window.h"


// Singleton WndClass
Window::WndClass Window::WndClass::m_WndClass;

Window::WndClass::WndClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
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
	m_IsCursorEnabled(true)
{
	// Calculate window size based on desired client region
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		// TODO: IMPLEMENT WINDOW EXCEPTION TYPE
		throw;
	}

	// Create window and get its handle
	m_Handle = CreateWindow(
		WndClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(wr.right - wr.left),
		(wr.bottom - wr.top),
		nullptr,
		nullptr,
		WndClass::GetInstance(),
		this);

	if (m_Handle == nullptr)
	{
		// TODO: IMPLEMENT WINDOW EXCEPTION TYPE
		throw;
	}

	ShowWindow(m_Handle, SW_SHOWDEFAULT);

	// Register mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // Mouse page
	rid.usUsage = 0x02; // Mouse page
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		// TODO: IMPLEMENT WINDOW EXCEPTION TYPE
		throw;
	}
}