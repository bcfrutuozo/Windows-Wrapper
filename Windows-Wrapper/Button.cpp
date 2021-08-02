#include "Button.h"

// Singleton WndClass
Button::BtnClass Button::BtnClass::m_BtnClass;

// Window class declarations
Button::BtnClass::BtnClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);

	RegisterClassEx(&wc);
}

Button::BtnClass::~BtnClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Button::BtnClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Button::BtnClass::GetInstance() noexcept
{
	return m_BtnClass.m_Instance;
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	IWinControl(parent, name, width, height, x, y)
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
	case WM_LBUTTONDOWN: OutputDebugString("aaa"); break;
	case BM_CLICK: break;
	case BM_GETCHECK: break;
	case BM_GETIMAGE: break;
	case BM_GETSTATE: break;
	case BM_SETCHECK: break;
	case BM_SETDONTCLICK: break;
	case BM_SETIMAGE: break;
	case BM_SETSTATE: break;
	case BM_SETSTYLE: break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;

		GetClientRect(hWnd, &rect);

		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 0));
		//SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, Text.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
		break;
	}
		
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Button::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		Button::BtnClass::GetName(),														// Class name
		Text.c_str(),																		// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_CENTER | BS_DEFPUSHBUTTON | WS_BORDER,		// Style values
		Location.X,																			// X position
		Location.Y,																			// Y position
		Size.Width,																			// Width
		Size.Height,																		// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),										// Parent handle
		(HMENU)1,																			// Menu handle
		Button::BtnClass::GetInstance(),													// Module instance handle
		this																				// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	//(WNDPROC)SetWindowLongPtr(static_cast<HWND>(Handle.ToPointer()), GWLP_WNDPROC, (LONG_PTR)&HandleMessageSetup);
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