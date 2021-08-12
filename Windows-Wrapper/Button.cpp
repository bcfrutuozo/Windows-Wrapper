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
	wc.hInstance = GetInstance();
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

Button::FlatButtonAppearance::FlatButtonAppearance()
	:
	BorderColor(Color::Border()),
	BorderSize(1),
	CheckedBackColor(Color::Default()),
	MouseDownBackColor(Color(204, 228, 247, 255)),
	MouseOverBackColor(Color(229, 241, 251, 255))
{

};

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
	case FlatStyle::Standard:	// Standard button MouseOver and Clicking effects are pre-defineds. User can only change the background color
	{
		if (IsClicking())		// m_IsTabSelected ALWAYS true when Clicking
		{
			// Draw outer border
			HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 84, 153));
			HGDIOBJ old_pen = SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			// Move inside rectangle for inner board
			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			// Draw dot for tabbed click and mouseover
			SetBkMode(hdc, OPAQUE);
			pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
			old_pen = SelectObject(hdc, pen);
			SetBkColor(hdc, RGB(204, 228, 247));
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//// Draw inner board
			pen = CreatePen(PS_INSIDEFRAME, 2, RGB(204, 228, 247));
			old_pen = SelectObject(hdc, pen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			// Move even more for background
			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//Clean up
			DeleteObject(old_pen);
			DeleteObject(pen);
		}
		else if (IsMouseOver())
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				c = Color::Selection();
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(c.GetR(), c.GetG(), c.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				SetBkMode(hdc, OPAQUE);
				pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
				old_pen = SelectObject(hdc, pen);
				SetBkColor(hdc, RGB(229, 241, 251));
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				DeleteObject(old_pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				c = Color::Selection();
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(c.GetR(), c.GetG(), c.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				c = Color(229, 241, 251, 255);
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				DeleteObject(old_pen);
				DeleteObject(pen);
			}
		}
		else
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				SetBkMode(hdc, OPAQUE);
				pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
				old_pen = SelectObject(hdc, pen);
				SetBkColor(hdc, RGB(0, 120, 215));
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				pen = CreatePen(PS_INSIDEFRAME, 0, RGB(225, 225, 225));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				DeleteObject(old_pen);
				DeleteObject(pen);
			}
			else
			{
				c = Color::Border();
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(c.GetR(), c.GetG(), c.GetB()));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				c = Color(225, 225, 225, 255);
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(c.GetR(), c.GetG(), c.GetB()));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				DeleteObject(old_pen);
				DeleteObject(pen);
			}
		}

		// Draw background inside
		HBRUSH background = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		FillRect(hdc, &rc, background);
		DeleteObject(background);

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
	HFONT hFont = CreateFont(Font.GetSizeInPixels(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());
	SelectObject(hdc, hFont);
	DrawText(hdc, GetText().c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hFont);

	// NEED TO IMPLEMENT A LARGE GRAPHICS CLASS TO HANDLE EVENTARGS TO THE USER.
	// IT'S PROBABLY GOING TO BE ONE OF THE LAST STEPS
	//Dispatch("OnPaint", new PaintEventArgs());

	EndPaint(hwnd, &ps);
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	WinControl(parent, name, width, height, x, y),
	FlatStyle(FlatStyle::Standard),
	FlatAppearance()
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