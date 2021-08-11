#include "Button.h"

// Singleton WndClass
Button::ButtonClass Button::ButtonClass::m_ButtonClass;

// Window class declarations
Button::ButtonClass::ButtonClass() noexcept
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

Button::ButtonClass::~ButtonClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Button::ButtonClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Button::ButtonClass::GetInstance() noexcept
{
	return m_ButtonClass.m_Instance;
}

int Button::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;	// To avoid flickering
}

void Button::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_RETURN:
	{
		SetClickingState(true);
		InvalidateRect(hwnd, nullptr, true);
		break;
	}
	case VK_TAB:	// Allows the user to change controls by pressing Tab
	{
		const WinControl* newCtl;

		// Previous Control
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			newCtl = GetPreviousControl();
		}
		else // Next control
		{
			newCtl = GetNextControl();

		}

		if (newCtl != nullptr)
		{
			SetFocus(static_cast<HWND>(newCtl->Handle.ToPointer()));
		}

		break;
	}
	}

	Dispatch("OnKeyDown", new KeyEventArgs(static_cast<Keys>(vk)));
}

void Button::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_RETURN:
	{
		Dispatch("OnClick", new EventArgs());
		SetClickingState(false);
		InvalidateRect(hwnd, nullptr, true);
		break;
	}
	}
}

void Button::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	SetBkMode(hdc, OPAQUE);
	RECT rc;
	GetClientRect(hwnd, &rc);

	//Create pen for button border
	HPEN pen = { 0 };

	// ONLY STANDARD BUTTON TYPE IS IMPLEMENTED
	// NEED TO DRAW FLAT, POPUP AND SYSTEM STYLES
	Color c;
	switch (FlatStyle)
	{
	case FlatStyle::Flat:
	{
		break;
	}
	case FlatStyle::Popup:
	{
		break;
	}
	case FlatStyle::Standard:
	{
		if (IsClicking())		// m_IsTabSelected ALWAYS true when Clicking
		{
			// Draw outer border
			c = Color::Selection();
			pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(c.GetR(), c.GetG(), c.GetB()));
			HGDIOBJ old_pen = SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			// Move inside rectangle for inner board
			rc.left += FlatAppearance.BorderSize;
			rc.top += FlatAppearance.BorderSize;
			rc.right -= FlatAppearance.BorderSize;
			rc.bottom -= FlatAppearance.BorderSize;

			// Draw inner board
			c = Color(229, 241, 251, 255);
			pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
			old_pen = SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			// Draw dot for tabbed click and mouseover
			pen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
			old_pen = SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			// Move even more for background
			rc.left += 2;
			rc.top += 2;
			rc.right -= 2;
			rc.bottom -= 2;

			// Draw background inside
			HBRUSH background = CreateSolidBrush(RGB(FlatAppearance.MouseDownBackColor.GetR(), FlatAppearance.MouseDownBackColor.GetG(), FlatAppearance.MouseDownBackColor.GetB()));
			FillRect(hdc, &rc, background);
			//Clean up
			SelectObject(hdc, old_pen);
			SelectObject(hdc, background);
			DeleteObject(pen);
		}
		else if (IsMouseOver())
		{
			m_Brush = CreateSolidBrush(RGB(FlatAppearance.MouseOverBackColor.GetR(), FlatAppearance.MouseOverBackColor.GetG(), FlatAppearance.MouseOverBackColor.GetB()));
			if (m_IsTabSelected)
			{
				c = Color::Selection();
				pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(c.GetR(), c.GetG(), c.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				HGDIOBJ old_brush = SelectObject(hdc, m_Brush);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				rc.left += FlatAppearance.BorderSize;
				rc.top += FlatAppearance.BorderSize;
				rc.right -= FlatAppearance.BorderSize;
				rc.bottom -= FlatAppearance.BorderSize;
				c = Color(229, 241, 251, 255);
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				pen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//Clean up
				SelectObject(hdc, old_pen);
				SelectObject(hdc, old_brush);
				DeleteObject(pen);
			}
			else
			{
				pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(FlatAppearance.BorderColor.GetR(), FlatAppearance.BorderColor.GetG(), FlatAppearance.BorderColor.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				HGDIOBJ old_brush = SelectObject(hdc, m_Brush);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				rc.left += FlatAppearance.BorderSize;
				rc.top += FlatAppearance.BorderSize;
				rc.right -= FlatAppearance.BorderSize;
				rc.bottom -= FlatAppearance.BorderSize;
				c = Color(229, 241, 251, 255);
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//Clean up
				SelectObject(hdc, old_pen);
				SelectObject(hdc, old_brush);
				DeleteObject(pen);
			}
		}
		else
		{
			m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
			if (m_IsTabSelected)
			{
				c = Color::Selection();
				pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(c.GetR(), c.GetG(), c.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				HGDIOBJ old_brush = SelectObject(hdc, m_Brush);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				rc.left += FlatAppearance.BorderSize;
				rc.top += FlatAppearance.BorderSize;
				rc.right -= FlatAppearance.BorderSize;
				rc.bottom -= FlatAppearance.BorderSize;
				c = Color(229, 241, 251, 255);
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//Clean up
				SelectObject(hdc, old_pen);
				SelectObject(hdc, old_brush);
				DeleteObject(pen);
			}
			else
			{
				pen = CreatePen(PS_INSIDEFRAME, FlatAppearance.BorderSize, RGB(FlatAppearance.BorderColor.GetR(), FlatAppearance.BorderColor.GetG(), FlatAppearance.BorderColor.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				HGDIOBJ old_brush = SelectObject(hdc, m_Brush);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				rc.left += FlatAppearance.BorderSize;
				rc.top += FlatAppearance.BorderSize;
				rc.right -= FlatAppearance.BorderSize;
				rc.bottom -= FlatAppearance.BorderSize;
				c = Color::Border();
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//Clean up
				SelectObject(hdc, old_pen);
				SelectObject(hdc, old_brush);
				DeleteObject(pen);
			}
		}
		break;
	}
	case FlatStyle::System:
	{
		break;
	}
	}

	//Select our brush into hDC
	//HGDIOBJ old_pen = SelectObject(hdc, pen);
	//HGDIOBJ old_brush = SelectObject(hdc, m_Brush);

	// GOING TO THINK A WAY OF IMPLEMENT NORMAL AND ROUNDED BORDER BUTTON

	//If you want rounded button, then use this, otherwise use FillRect().

	//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);

	//FillRect(hdc, &rc, m_Brush);



	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
	HFONT hFont = CreateFont(Font.GetSize(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());
	SelectObject(hdc, hFont);
	DrawText(hdc, GetText().c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hFont);

	EndPaint(hwnd, &ps);
	// NEED TO IMPLEMENT A LARGE GRAPHICS CLASS TO HANDLE EVENTARGS TO THE USER.
	// IT'S PROBABLY GOING TO BE ONE OF THE LAST STEPS
	//Dispatch("OnPaint", new PaintEventArgs());
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	WinControl(parent, name, width, height, x, y),
	FlatStyle(FlatStyle::Standard)
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
		ButtonClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,				// Style values
		Location.X,										// X position
		Location.Y,										// Y position
		Size.Width,										// Width
		Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		NULL,						                	// Menu handle
		ButtonClass::GetInstance(),						// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
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