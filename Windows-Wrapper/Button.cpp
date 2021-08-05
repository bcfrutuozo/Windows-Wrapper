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
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
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

void Button::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		BtnClass::GetName(),																				// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE,																				// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		Size.Width,																							// Width
		Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		NULL,						                														// Menu handle
		BtnClass::GetInstance(),																			// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
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

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI Button::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	OutputDebugString(Mapper(msg, wParam, lParam).c_str());

	switch (msg)
	{
	case WM_SETCURSOR:  break;
	case WM_CREATE: break;
	case WM_ERASEBKGND: break;
	case WM_COMMAND:  break;
	case WM_CLOSE:  return 0;		// Exit message to be handled in application class
	case WM_DESTROY:  break;
	case WM_NCDESTROY:  break;
	case WM_SETFOCUS: Dispatch("OnGotFocus", new EventArgs());  break;
	case WM_KILLFOCUS: Dispatch("OnLostFocus", new EventArgs()); break;
	case WM_ACTIVATE:  break;
		/******************** KEYBOARD MESSAGES *********************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		Dispatch("OnKeyDown", new KeyEventArgs(static_cast<Keys>(wParam)));
	}
	break;	// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		Dispatch("OnKeyUp", new KeyEventArgs(static_cast<Keys>(wParam)));
	}
	break;		// Syskey commands need to be handled to track ALT key (VK_MENU)
	case WM_CHAR:
	{
		Dispatch("OnKeyPress", new KeyPressEventArgs(static_cast<unsigned char>(wParam)));
	}
	break;
	/******************* END KEYBOARD MESSAGES ******************/
	/********************** MOUSE MESSAGES **********************/
	case WM_MOUSEMOVE:
	{
		if (!IsMouseOver)
		{
			OutputDebugString("ENTROUUU");
			Dispatch("OnMouseEnter", new EventArgs());
		}

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT); //Monitor mouse to leave
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = static_cast<HWND>(Handle.ToPointer());
		if (::_TrackMouseEvent(&tme))
		{
			IsMouseOver = true;
			InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
		}

		Dispatch("OnMouseMove", new MouseEventArgs(MouseButtons::None, 0, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_LBUTTONDOWN:
	{
		IsMouseOver = true;
		IsClicking = true;
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
		Dispatch("OnMouseDown", new MouseEventArgs(MouseButtons::Left, 1, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_MOUSELEAVE:
	{
		IsClicking = false;
		IsMouseOver = false;
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
		Dispatch("OnMouseLeave", new EventArgs());
	}
	break;
	case WM_LBUTTONUP:
	{
		IsClicking = false;
		IsMouseOver = true;
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, TRUE);
		Dispatch("OnClick", new EventArgs());
		Dispatch("OnMouseClick", new MouseEventArgs(MouseButtons::Left, 1, 0, LOWORD(lParam), HIWORD(lParam)));
		Dispatch("OnMouseUp", new MouseEventArgs(MouseButtons::Left, 1, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_RBUTTONDOWN:
	{
		Dispatch("OnMouseDown", new MouseEventArgs(MouseButtons::Right, 1, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_RBUTTONUP:
	{
		Dispatch("OnMouseUp", new MouseEventArgs(MouseButtons::Right, 1, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		Dispatch("OnDoubleClick", new EventArgs());
		Dispatch("OnMouseDoubleClick", new MouseEventArgs(MouseButtons::Left, 2, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	case WM_RBUTTONDBLCLK:
	{
		Dispatch("OnMouseDoubleClick", new MouseEventArgs(MouseButtons::Right, 2, 0, LOWORD(lParam), HIWORD(lParam)));
	}
	break;
	case WM_MOUSEWHEEL:
	{
		switch (GET_KEYSTATE_WPARAM(wParam))
		{
		case MK_LBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Left, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam))); break;
		case MK_MBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Middle, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam))); break;
		case MK_RBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Right, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam)));  break;
		case MK_XBUTTON1: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::XButton1, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam))); break;
		case MK_XBUTTON2: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::XButton2, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam))); break;
		default: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::None, 0, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(lParam), HIWORD(lParam))); break;
		}
	}
	break;
	case WM_SHOWWINDOW:
	{
		Dispatch("OnVisibleChanged", new EventArgs());
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(static_cast<HWND>(Handle.ToPointer()), &ps);
		RECT rc;
		GetClientRect(static_cast<HWND>(Handle.ToPointer()), &rc);

		//Create pen for button border
		HPEN pen;

		if (IsClicking)
		{
			pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(FlatAppearance.BorderColor.GetR(), FlatAppearance.BorderColor.GetG(), FlatAppearance.BorderColor.GetB()));
			m_Brush = CreateSolidBrush(RGB(FlatAppearance.MouseDownBackColor.GetR(), FlatAppearance.MouseDownBackColor.GetG(), FlatAppearance.MouseDownBackColor.GetB()));
		}
		else if (IsMouseOver)
		{
			pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(FlatAppearance.BorderColor.GetR(), FlatAppearance.BorderColor.GetG(), FlatAppearance.BorderColor.GetB()));
			m_Brush = CreateSolidBrush(RGB(FlatAppearance.MouseOverBackColor.GetR(), FlatAppearance.MouseOverBackColor.GetG(), FlatAppearance.MouseOverBackColor.GetB()));
		}
		else
		{
			pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(FlatAppearance.BorderColor.GetR(), FlatAppearance.BorderColor.GetG(), FlatAppearance.BorderColor.GetB()));
			m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		}

		//Select our brush into hDC
		HGDIOBJ old_pen = SelectObject(hdc, pen);
		HGDIOBJ old_brush = SelectObject(hdc, m_Brush);

		// GOING TO THINK A WAY OF IMPLEMENT NORMAL AND ROUNDED BORDER BUTTON

		//If you want rounded button, then use this, otherwise use FillRect().

		//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		//FillRect(hdc, &rc, m_Brush);

		//Clean up
		SelectObject(hdc, old_pen);
		SelectObject(hdc, old_brush);
		DeleteObject(pen);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, m_ForeColor.ToRGB());
		DrawText(hdc, GetText().c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		// NEED TO IMPLEMENT A LARGE GRAPHICS CLASS TO HANDLE EVENTARGS TO THE USER.
		// IT'S PROBABLY GOING TO BE ONE OF THE LAST STEPS
		//Dispatch("OnPaint", new PaintEventArgs());

		break;
	}
	break;
	/******************** END MOUSE MESSAGES ********************/
	/******************** RAW MOUSE MESSAGES ********************/
	case WM_INPUT: break;
		/****************** END RAW MOUSE MESSAGES ******************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}