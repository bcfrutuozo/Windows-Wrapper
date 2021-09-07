#include "ListBox.h"

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_DOWN:
	{
		if (m_SelectedIndex < GetDataSource()->GetCount())
		{
			++m_SelectedIndex;
		}

		break;
	}
	case VK_UP:
	{
		if (m_SelectedIndex > 0)
		{
			--m_SelectedIndex;
		}

		break;
	}
	}

	Update();
	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void ListBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	size_t i = 0;

	for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
	{
		if (y >= it->top && y <= it->bottom && x >= it->left && x <= it->right)
		{
			SetSelectedIndex(i);
			break;
		}
	}

	Control::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void ListBox::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdcMem, &ps.rcPaint, bgColor);
	SelectObject(hdcMem, bgColor);
	DeleteObject(bgColor);

	HFONT hFont = CreateFont(
		m_Font.GetSizeInPixels(),
		0,
		0,
		0,
		m_Font.IsBold() ? FW_BOLD : FW_NORMAL,
		m_Font.IsItalic(),
		m_Font.IsUnderline(),
		m_Font.IsStrikeOut(),
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_Font.GetName().c_str());

	SelectObject(hdcMem, hFont);

	RECT r;

	r.left = r.top = 0;
	r.right = m_Size.Width;
	r.bottom = m_Size.Height;

	switch (m_BorderStyle)
	{
	case BorderStyle::None:
	{
		break;
	}
	case BorderStyle::FixedSingle:
	{
		if (m_IsTabSelected)
		{
			// Draw outer border
			HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 0, 0));
			HGDIOBJ old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, r.left, r.top, r.right, r.bottom);

			// Move inside rectangle for inner board
			r.left += 1;
			r.top += 1;
			r.right -= 1;
			r.bottom -= 1;

			//Clean up
			SelectObject(hdcMem, old_pen);
			DeleteObject(old_pen);
			SelectObject(hdcMem, pen);
			DeleteObject(pen);
		}
		else
		{
			if (IsMouseOver())
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, r.left, r.top, r.right, r.bottom);

				// Move inside rectangle for inner board
				r.left += 1;
				r.top += 1;
				r.right -= 1;
				r.bottom -= 1;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
			else
			{
				// Draw outer border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(122, 122, 122));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				Rectangle(hdcMem, r.left, r.top, r.right, r.bottom);

				// Move inside rectangle for inner board
				r.left += 1;
				r.top += 1;
				r.right -= 1;
				r.bottom -= 1;

				//Clean up
				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}
		}
		break;
	}
	case BorderStyle::Fixed3D:
	{
		// Draw outer border
		HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(130, 135, 144));
		HGDIOBJ old_pen = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, r.left, r.top, r.right, r.bottom);

		// Move inside rectangle for inner board
		r.left += 1;
		r.top += 1;
		r.right -= 1;
		r.bottom -= 1;

		// Draw outer border
		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(255, 255, 255));
		old_pen = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, r.left, r.top, r.right, r.bottom);

		// Move inside rectangle for inner board
		r.left += 1;
		r.top += 1;
		r.right -= 1;
		r.bottom -= 1;

		//Clean up
		SelectObject(hdcMem, old_pen);
		DeleteObject(old_pen);
		SelectObject(hdcMem, pen);
		DeleteObject(pen);

		break;
	}
	}

	// Example test draw with the desired font to calculate each ListBox item size
	SIZE m_SingleSize;
	const char* verifier = "A";
	GetTextExtentPoint32(hdcMem, verifier, 2, &m_SingleSize);
	SetSingleHorizontalIncrement(m_SingleSize.cx);
	SetSingleVerticalIncrement(m_SingleSize.cy);

	// Drawable block inside ListBox
	RECT rc(m_Margin.Left + r.left, m_Margin.Top + r.top, r.right - m_Margin.Right, r.bottom - m_Margin.Bottom);
	const auto& dataSource = GetDataSource();

	if (m_IsRebinding)
	{
		m_RowPosition.clear();
		if (m_RowPosition.size() < dataSource->GetCount())
		{
			m_RowPosition.resize(dataSource->GetCount());
		}
	}

	size_t i = 0;
	for (auto it = dataSource->begin(); it != dataSource->end(); ++it, ++i)
	{
		if (m_IsMultiColumn)
		{
			int columnNumber = (m_Size.Width - m_Margin.Right) / m_SingleSize.cx;
		}
		else
		{
			if ((static_cast<unsigned long long>(rc.bottom) - rc.top) < (m_SingleSize.cy * dataSource->GetCount()))
			{
				VerticalScrollBar.Show();
				VerticalScrollBar.SetMaximumValue(dataSource->GetCount());
			}
			else
			{
				VerticalScrollBar.Hide();
				VerticalScrollBar.SetMaximumValue(0);
			}

			RECT cr;
			CopyRect(&cr, &rc);
			cr.top = (m_SingleSize.cy * i) - (VerticalScrollBar.GetScrolling() * m_SingleSize.cy) + rc.top;
			cr.bottom = (cr.top + m_SingleSize.cy);
			cr.right -= m_Margin.Right;

			if (VerticalScrollBar.IsShown())
			{
				cr.right -= VerticalScrollBar.GetSize().Width;
			}
			
			m_RowPosition[i] = cr;

			if (cr.bottom < m_Size.Height && cr.top >= rc.top)
			{
				if (m_SelectedIndex == i)
				{
					SetBkColor(hdcMem, RGB(0, 120, 215));
					SetTextColor(hdcMem, RGB(255, 255, 255));
					// Draw background
					HBRUSH brush = CreateSolidBrush(RGB(0, 120, 215));
					FillRect(hdcMem, &cr, brush);
					SelectObject(hdcMem, brush);
					DeleteObject(brush);
				}
				else
				{
					SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
				}

				DrawText(hdcMem, (*dataSource)[i]->Value.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
				DrawText(hdcMem, Text.c_str(), static_cast<int>((*dataSource)[i]->Value.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
			}
		}
	}

	auto totalInScreen = m_Size.Height / m_SingleSize.cy;

	if (VerticalScrollBar.IsShown() && m_IsRebinding)
	{
		HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_RowPosition.back().right - m_RowPosition.back().left, totalInScreen + 1));
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(ps.hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hFont);
	DeleteObject(hFont);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);

	EndPaint(hwnd, &ps);
}

void ListBox::IncrementHorizontalScroll() noexcept
{
	if (!IsHorizontalScrollEnabled())
	{
		return;
	}

	auto inc = GetSingleHorizontalIncrement();
	for (auto& r : m_RowPosition)
	{
		r.left += inc;
		r.right += inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &r, false);
	}
}

void ListBox::DecrementHorizontalScroll() noexcept
{
	if (!IsHorizontalScrollEnabled())
	{
		return;
	}

	auto inc = GetSingleHorizontalIncrement();
	for (auto& r : m_RowPosition)
	{
		r.left -= inc;
		r.right -= inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &r, false);
	}
}

void ListBox::IncrementVerticalScroll() noexcept
{
	if (!IsVerticalScrollEnabled())
	{
		return;
	}

	auto inc = GetSingleVerticalIncrement();
	for (auto& r : m_RowPosition)
	{
		r.bottom += inc;
		r.top += inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &r, false);
	}
}

void ListBox::DecrementVerticalScroll() noexcept
{
	if (!IsVerticalScrollEnabled())
	{
		return;
	}

	auto inc = GetSingleVerticalIncrement();
	for (auto& r : m_RowPosition)
	{
		r.bottom -= inc;
		r.top -= inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &r, false);
	}
}

ListBox::ListBox(Control* parent, int width, int height, int x, int y)
	:
	ListControl(parent, "", width, height, x, y),
	m_IsMultiColumn(false),
	m_IsHorizontalScrollVisible(false),
	m_IsScrollAlwaysVisible(false),
	m_SelectionMode(SelectionMode::Single),
	m_DockStyle(DockStyle::None),
	m_BorderStyle(BorderStyle::Fixed3D)
{

}

ListBox::~ListBox()
{

}

BorderStyle ListBox::GetBorderStyle() const noexcept
{
	return m_BorderStyle;
}

void ListBox::SetBorderStyle(BorderStyle style) noexcept
{
	m_BorderStyle = style;
	Update();
}