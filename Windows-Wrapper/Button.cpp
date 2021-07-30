#include "Button.h"

// Singleton WinButton
Button::WinButton Button::WinButton::m_WinButton;

// Window class declarations
Button::WinButton::WinButton() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_PARENTDC | CS_NOCLOSE | CS_HREDRAW | CS_VREDRAW;
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

Button::WinButton::~WinButton()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Button::WinButton::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Button::WinButton::GetInstance() noexcept
{
	return m_WinButton.m_Instance;
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y)
{
	Initialize();
}

Button::~Button()
{

}

LRESULT Button::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case BM_CLICK: break;
	case BM_GETCHECK: break;
	case BM_GETIMAGE: break;
	case BM_GETSTATE: break;
	case BM_SETCHECK: break;
	case BM_SETDONTCLICK: break;
	case BM_SETIMAGE: break;
	case BM_SETSTATE: break;
	case BM_SETSTYLE: break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Button::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		WinButton::GetName(),																// Class name
		Text.c_str(),																		// Window title
		WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON	,								// Style values
		Location.X,																			// X position
		Location.Y,																			// Y position
		Size.Width,																			// Width
		Size.Height,																		// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),										// Parent handle
		nullptr,																			// Menu handle
		WinButton::GetInstance(),															// Module instance handle
		this																				// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Creates and show the button
	Show();
}

void Button::Disable() noexcept
{

}

void Button::Enable() noexcept
{

}

void Button::Hide()
{
	if (IsShown())
	{
		IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void Button::Show()
{
	if (!IsShown())
	{
		IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}