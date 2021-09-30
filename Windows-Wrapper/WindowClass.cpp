#include "WindowClass.h"
#include "WinAPI.h"
#include "Color.h"

// Singleton WndClass
WindowClass WindowClass::m_WindowClass;

// Window class declarations
WindowClass::WindowClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinAPI::HandleMessageSetup;
	wc.hInstance = GetInstance();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);
}

WindowClass::~WindowClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* WindowClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE WindowClass::GetInstance() noexcept
{
	return m_WindowClass.m_Instance;
}