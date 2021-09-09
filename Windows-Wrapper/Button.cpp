#include "Button.h"

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
		Update();
		break;
	}
	}

	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void Button::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_RETURN:
	{
		Dispatch("OnClick", &ArgsDefault);
		SetClickingState(false);
		Update();
		break;
	}
	}

	Control::OnKeyUp_Impl(hwnd, vk, cRepeat, flags);
}

void Button::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	SetBkMode(ps.hdc, OPAQUE);
	RECT rc;
	GetClientRect(hwnd, &rc);

	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	// ONLY STANDARD BUTTON TYPE IS IMPLEMENTED
	// NEED TO DRAW FLAT, POPUP AND SYSTEM STYLES
	switch (m_FlatStyle)
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
			HGDIOBJ old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

			// Move inside rectangle for inner board
			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			// Draw dot for tabbed click and mouseover
			SetBkMode(hdcMem, OPAQUE);
			pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
			old_pen = SelectObject(hdcMem, pen);
			SetBkColor(hdcMem, RGB(204, 228, 247));
			Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//// Draw inner board
			pen = CreatePen(PS_INSIDEFRAME, 2, RGB(204, 228, 247));
			old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

			// Move even more for background
			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//Clean up
			SelectObject(hdcMem, old_pen);
			DeleteObject(old_pen);
			SelectObject(hdcMem, pen);
			DeleteObject(pen);
		}
		else if (IsMouseOver())
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				SetBkMode(hdcMem, OPAQUE);
				pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
				old_pen = SelectObject(hdcMem, pen);
				SetBkColor(hdcMem, RGB(229, 241, 251));
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
		}
		else
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				SetBkMode(hdcMem, OPAQUE);
				pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
				old_pen = SelectObject(hdcMem, pen);
				SetBkColor(hdcMem, RGB(0, 120, 215));
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				pen = CreatePen(PS_INSIDEFRAME, 0, RGB(225, 225, 225));
				old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(173, 173, 173));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(225, 225, 225));
				old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
		}

		// Draw background inside
		HBRUSH background = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		FillRect(hdcMem, &rc, background);
		SelectObject(hdcMem, background);
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

	SetBkMode(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
	HFONT hFont = CreateFont(m_Font.GetSizeInPixels(),
		0,
		0, 
		0, 
		m_Font.IsBold() ? FW_BOLD : FW_NORMAL, 
		m_Font.IsItalic(), 
		m_Font.IsUnderline(), 
		m_Font.IsStrikeOut(), 
		ANSI_CHARSET,
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, 
		m_Font.GetName().c_str());
	SelectObject(hdcMem, hFont);
	DrawText(hdcMem, GetText().c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(hFont);

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(ps.hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	// NEED TO IMPLEMENT A LARGE GRAPHICS CLASS TO HANDLE EVENTARGS TO THE USER.
	// IT'S PROBABLY GOING TO BE ONE OF THE LAST STEPS
	//Dispatch("OnPaint", new PaintEventArgs());

	SelectObject(ps.hdc, hbmMem);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
	EndPaint(hwnd, &ps);
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	m_FlatStyle(FlatStyle::Standard),
	m_FlatAppearance()
{
	Initialize();
}

Button::~Button()
{

}

void Button::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,				// Style values
		m_Location.X,										// X position
		m_Location.Y,										// Y position
		m_Size.Width,									// Width
		m_Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		nullptr,					                	// Menu handle
		WindowClass::GetInstance(),						// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
}

FlatButtonAppearance Button::GetFlatButtonAppearance() const noexcept
{
	return m_FlatAppearance;
}

void Button::SetFlatButtonAppearance(FlatButtonAppearance appearance) noexcept
{
	m_FlatAppearance = appearance;
	Update();
}

FlatStyle Button::GetFlatStyle() const noexcept
{
	return m_FlatStyle;
}

void Button::SetFlatStyle(FlatStyle style) noexcept
{
	m_FlatStyle = style;
	Update();
}
