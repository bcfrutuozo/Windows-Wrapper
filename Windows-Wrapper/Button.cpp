#include "Button.h"
#include "ControlException.h"

void Button::DrawBorder(HDC& hdc, RECT& rc)
{
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
	case FlatStyle::Standard_Windows10:	// Standard button MouseOver and Clicking effects are pre-defineds. User can only change the background color
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
			SelectObject(hdc, old_pen);
			DeleteObject(old_pen);
			SelectObject(hdc, pen);
			DeleteObject(pen);
		}
		else if (IsMouseOver())
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
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
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
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
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(173, 173, 173));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				pen = CreatePen(PS_INSIDEFRAME, 2, RGB(225, 225, 225));
				old_pen = SelectObject(hdc, pen);
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
				DeleteObject(pen);
			}
		}

		break;
	}
	case FlatStyle::Standard_Windows11:
	{
		if (IsClicking())		// m_IsTabSelected ALWAYS true when Clicking
		{
			// Draw outer border
			HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 84, 153));
			HGDIOBJ old_pen = SelectObject(hdc, pen);
			//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

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
			//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//// Draw inner board
			//pen = CreatePen(PS_INSIDEFRAME, 2, RGB(204, 228, 247));
			//old_pen = SelectObject(hdc, pen);
			////Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

			// Move even more for background
			rc.left += 1;
			rc.top += 1;
			rc.right -= 1;
			rc.bottom -= 1;

			//Clean up
			SelectObject(hdc, old_pen);
			DeleteObject(old_pen);
			SelectObject(hdc, pen);
			DeleteObject(pen);
		}
		else if (IsMouseOver())
		{
			if (m_IsTabSelected)
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

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
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//// Draw inner board
				//pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				//old_pen = SelectObject(hdc, pen);
				////Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				//pen = CreatePen(PS_INSIDEFRAME, 2, RGB(229, 241, 251));
				//old_pen = SelectObject(hdc, pen);
				////Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
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
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

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
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw dot for tabbed click and mouseover
				//pen = CreatePen(PS_INSIDEFRAME, 0, RGB(225, 225, 225));
				//old_pen = SelectObject(hdc, pen);
				////Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

				// Move even more for background
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
				DeleteObject(pen);
			}
			else
			{
				SetBkMode(hdc, TRANSPARENT);
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 2, RGB(173, 173, 173));
				HGDIOBJ old_pen = SelectObject(hdc, pen);
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 15, 15);

				// Move inside rectangle for inner board
				rc.left += 1;
				rc.top += 1;
				rc.right -= 1;
				rc.bottom -= 1;

				// Draw inner board
				//pen = CreatePen(PS_INSIDEFRAME, 2, RGB(225, 225, 225));
				//old_pen = SelectObject(hdc, pen);
				////Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 25, 25);

				// Move even more for background
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

				//Clean up
				SelectObject(hdc, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdc, pen);
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
}

void Button::Draw(const Graphics& graphics, Drawing::Rectangle rectangle)
{
	auto hwnd = static_cast<HWND>(Handle.ToPointer());
	auto hdc = static_cast<HDC>(graphics.GetHDC().ToPointer());
	
	SetBkMode(hdc, OPAQUE);
	RECT rc;
	GetClientRect(hwnd, &rc);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	// Select current font
	auto hFont = Fonts->find(m_Font.ToString());
	SelectObject(hdcMem, hFont->second);

	// Draw background inside before drawing borders to round rectangle
	HBRUSH background = CreateSolidBrush(GetBackgroundColor().ToRGB());
	FillRect(hdcMem, &rc, background);
	SelectObject(hdcMem, background);
	DeleteObject(background);

	DrawBorder(hdcMem, rc);

	SetBkMode(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
	DrawText(hdcMem, GetText().c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdc, hbmMem);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
}

int Button::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	return 1;	// To avoid flickering
}

void Button::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
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

void Button::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
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

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	m_FlatStyle(FlatStyle::Standard_Windows11),
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
