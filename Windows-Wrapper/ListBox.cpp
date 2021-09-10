#include "ListBox.h"

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_DOWN:
	{
		if (m_SelectedIndex < GetDataSource()->GetCount() - 1)
		{
			++m_SelectedIndex;

			if (m_RowPosition[m_SelectedIndex].bottom > m_DrawableArea.bottom)
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].top < m_DrawableArea.top)
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
			}
		}
		else
		{
			HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
		}

		break;
	}
	case VK_UP:
	{
		if (m_SelectedIndex > 0)
		{
			--m_SelectedIndex;

			if (m_RowPosition[m_SelectedIndex].bottom > m_DrawableArea.bottom)
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].top < m_DrawableArea.top)
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
			}
		}
		else
		{
			HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
		}

		break;
	}
	case VK_LEFT:
	{
		if (!m_IsMultiColumn)
		{
			break;
		}

		if (m_SelectedIndex > m_RowNumber - 1)
		{
			m_SelectedIndex -= m_RowNumber;

			if (m_RowPosition[m_SelectedIndex].right > m_DrawableArea.right)
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].left < m_DrawableArea.left)
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
			}
		}
		else
		{
			HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
		}

		break;
	}

	case VK_RIGHT:
	{
		if (!m_IsMultiColumn)
		{
			break;
		}

		if (m_SelectedIndex < GetDataSource()->GetCount() - m_RowNumber)
		{
			int old = m_SelectedIndex;
			m_SelectedIndex += m_RowNumber;

			int c = 0;

			if (m_RowPosition[m_SelectedIndex].right > m_DrawableArea.right)
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].left < m_DrawableArea.left)
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
			}
		}
		else
		{
			HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
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

	if (y >= m_DrawableArea.top && y <= m_DrawableArea.bottom && x >= m_DrawableArea.left && x <= m_DrawableArea.right)
	{
		for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
		{
			if (y >= it->top && y <= it->bottom && x >= it->left && x <= it->right)
			{
				SetSelectedIndex(i);
				break;
			}
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
	SetMinimumItemWidth(m_SingleSize.cx);
	SetItemWidth(m_IsMultiColumn ? m_ColumnWidth : m_SingleSize.cx);
	SetItemHeight(m_SingleSize.cy);

	// Drawable block inside ListBox
	m_DrawableArea.left = m_Margin.Left + r.left;
	m_DrawableArea.top = m_Margin.Top + r.top;
	m_DrawableArea.right = r.right - m_Margin.Right;
	m_DrawableArea.bottom = r.bottom - m_Margin.Bottom;

	const auto& dataSource = GetDataSource();

	if (m_IsRebinding)
	{
		// Horizontal ScrollBar
		if (m_IsMultiColumn)
		{
			size_t currentSizeX = 0;
			auto totalSizeX = m_DrawableArea.right - m_DrawableArea.left;

			while (currentSizeX < totalSizeX)
			{
				currentSizeX += GetItemWidth();
			}

			if (currentSizeX != totalSizeX)
			{
				currentSizeX += m_Margin.Left + m_Margin.Right;
				switch (m_BorderStyle)
				{
				case BorderStyle::None:
				{
					break;
				}
				case BorderStyle::FixedSingle:
				{
					// 1 on top and 1 on bottom
					currentSizeX += 2;
					break;
				}
				case BorderStyle::Fixed3D:
				{
					// 2 on top and 2 on bottom
					currentSizeX += 4;
					break;
				}
				}

				int oldHeightX = m_Size.Width;
				m_Size.Width = currentSizeX;
				Resize(currentSizeX, m_Size.Height + 1); // Height + 1 is because of the HorizontalScrollBar must have at least one extra pixel to draw the inner window
				auto hSize = HorizontalScrollBar.GetSize();
				hSize.Width += (currentSizeX - oldHeightX);

				auto scrollLoc = HorizontalScrollBar.GetLocation();
				scrollLoc.Y += 1;	// Increment location by 1 because it has a single pixel for the inner windows
				HorizontalScrollBar.SetLocation(scrollLoc);
				HorizontalScrollBar.Resize(hSize);

				EndPaint(hwnd, &ps);
				OnPaint_Impl(hwnd);
				return;
			}
		}

		// Vertical ScrollBar
		else
		{
			size_t currentSizeY = 0;
			auto totalSizeY = m_DrawableArea.bottom - m_DrawableArea.top;

			while (currentSizeY < totalSizeY)
			{
				currentSizeY += GetItemHeight();
			}

			if (currentSizeY != totalSizeY)
			{
				currentSizeY += m_Margin.Top + m_Margin.Bottom;
				switch (m_BorderStyle)
				{
				case BorderStyle::None:
				{
					break;
				}
				case BorderStyle::FixedSingle:
				{
					// 1 on top and 1 on bottom
					currentSizeY += 2;
					break;
				}
				case BorderStyle::Fixed3D:
				{
					// 2 on top and 2 on bottom
					currentSizeY += 4;
					break;
				}
				}

				int oldHeightY = m_Size.Height;
				m_Size.Height = currentSizeY;
				Resize(m_Size.Width, currentSizeY);
				auto vSize = VerticalScrollBar.GetSize();
				vSize.Height += (currentSizeY - oldHeightY);
				VerticalScrollBar.Resize(vSize);

				EndPaint(hwnd, &ps);
				OnPaint_Impl(hwnd);
				return;
			}
		}


		// This block will only be executed once after resize
		m_RowPosition.clear();
		if (m_RowPosition.size() < dataSource->GetCount())
		{
			m_RowPosition.resize(dataSource->GetCount());
		}

		m_TotalItemsInDrawableArea = 0;
	}

	bool hEnabled = false;
	bool vEnabled = false;
	m_RowNumber = (m_DrawableArea.bottom - m_DrawableArea.top) / GetItemHeight();
	m_ColumnNumber = (static_cast<unsigned long long>(m_DrawableArea.right) - m_DrawableArea.left) / GetItemWidth();

	if (m_IsMultiColumn)
	{
		if ((static_cast<unsigned long long>(m_DrawableArea.right) - m_DrawableArea.left) < (GetItemWidth() * m_ColumnNumber + 1))
		{
			HorizontalScrollBar.SetMaximumValue(static_cast<size_t>(std::ceil(static_cast<float>(dataSource->GetCount()) / static_cast<float>((m_RowNumber)))));
			hEnabled = true;
		}
		else
		{
			HorizontalScrollBar.SetMaximumValue(0);
			hEnabled = false;
		}

		// Reduce HorizontalScroll before getting correct positions
		if (hEnabled)
		{
			m_DrawableArea.bottom -= HorizontalScrollBar.GetSize().Height;
		}
	}
	else
	{
		if ((static_cast<unsigned long long>(m_DrawableArea.bottom) - m_DrawableArea.top) < (GetItemHeight() * dataSource->GetCount()))
		{
			VerticalScrollBar.SetMaximumValue((dataSource->GetCount()));
			vEnabled = true;
		}
		else
		{
			VerticalScrollBar.SetMaximumValue(0);
			vEnabled = false;
		}
	}

	size_t i = 0;
	size_t i2 = -1;
	for (auto it = dataSource->begin(); it != dataSource->end(); ++it, ++i)
	{
		if (m_IsMultiColumn)
		{
			// HORRORIZONTAL

			RECT cr;
			CopyRect(&cr, &m_DrawableArea);

			int r = i % m_RowNumber;
			if (r == 0) ++i2;

			cr.top = (GetItemHeight() * r) + m_DrawableArea.top;
			cr.bottom = (cr.top + GetItemHeight());
			cr.left = + m_DrawableArea.left - (HorizontalScrollBar.GetScrolling() * GetItemWidth()) + (GetItemWidth() * i2);
			cr.right = cr.left + GetItemWidth() - m_Margin.Right;

			m_RowPosition[i] = cr;

			if (cr.left >= m_DrawableArea.left && cr.right <= m_DrawableArea.right)
			{
				if (m_IsRebinding && r == 0)
				{
					++m_TotalItemsInDrawableArea;
				}

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

			// HORRORIZONTAL
		}
		else
		{
			RECT cr;
			CopyRect(&cr, &m_DrawableArea);
			cr.top = (GetItemHeight() * i) - (VerticalScrollBar.GetScrolling() * GetItemHeight()) + m_DrawableArea.top;
			cr.bottom = (cr.top + GetItemHeight());
			cr.right -= m_Margin.Right;

			if (vEnabled)
			{
				cr.right -= VerticalScrollBar.GetSize().Width;
			}

			m_RowPosition[i] = cr;

			if (cr.bottom <= m_DrawableArea.bottom && cr.top >= m_DrawableArea.top)
			{
				if (m_IsRebinding)
				{
					++m_TotalItemsInDrawableArea;
				}

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

	if (m_IsMultiColumn)
	{
		if (hEnabled)
		{
			EnableHorizontalScroll();

			if (m_IsRebinding)
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_TotalItemsInDrawableArea, 0));
				m_IsRebinding = false;
			}
		}
		else
		{
			DisableHorizontalScroll();
		}
	}
	else
	{
		if (vEnabled)
		{
			EnableVerticalScroll();

			if (m_IsRebinding)
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_RowPosition.back().right - m_RowPosition.back().left, m_TotalItemsInDrawableArea));
				m_IsRebinding = false;
			}
		}
		else
		{
			DisableVerticalScroll();
		}
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

	auto inc = GetItemWidth();
	for (auto& r : m_RowPosition)
	{
		r.left += inc;
		r.right += inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &m_DrawableArea, false);
	}
}

void ListBox::DecrementHorizontalScroll() noexcept
{
	if (!IsHorizontalScrollEnabled())
	{
		return;
	}

	auto inc = GetItemWidth();
	for (auto& r : m_RowPosition)
	{
		r.left -= inc;
		r.right -= inc;

		// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &m_DrawableArea, false);
	}
}

void ListBox::IncrementVerticalScroll() noexcept
{
	if (!IsVerticalScrollEnabled())
	{
		return;
	}

	auto inc = GetItemHeight();
	for (auto& r : m_RowPosition)
	{
		r.bottom += inc;
		r.top += inc;
	}

	// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
	RECT tmp;
	CopyRect(&tmp, &m_DrawableArea);
	tmp.right -= VerticalScrollBar.GetSize().Width;
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &tmp, false);
}

void ListBox::DecrementVerticalScroll() noexcept
{
	if (!IsVerticalScrollEnabled())
	{
		return;
	}

	auto inc = GetItemHeight();
	for (auto& r : m_RowPosition)
	{
		r.bottom -= inc;
		r.top -= inc;
	}

	// Invalidates only the item entry region. Otherwise scrollbar would flicker because it's a control inside a control being redraw all the time.
	RECT tmp;
	CopyRect(&tmp, &m_DrawableArea);
	tmp.right -= VerticalScrollBar.GetSize().Width;
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), &tmp, false);
}

ListBox::ListBox(Control* parent, int width, int height, int x, int y)
	:
	ListControl(parent, "", width, height, x, y),
	m_IsMultiColumn(false),
	m_IsHorizontalScrollVisible(false),
	m_IsScrollAlwaysVisible(false),
	m_SelectionMode(SelectionMode::Single),
	m_DockStyle(DockStyle::None),
	m_BorderStyle(BorderStyle::Fixed3D),
	m_DrawableArea({ 0 }),
	m_TotalItemsInDrawableArea(0),
	m_ColumnWidth(120)
{

}

ListBox::~ListBox()
{

}

bool ListBox::IsMultiColumn() const noexcept
{
	return m_IsMultiColumn;
}

void ListBox::EnableMultiColumn() noexcept
{
	if (!m_IsMultiColumn)
	{
		m_IsMultiColumn = true;
		Update();
	}
}

void ListBox::DisableMultiColumn() noexcept
{
	if (m_IsMultiColumn)
	{
		m_IsMultiColumn = false;
		Update();
	}
}

size_t ListBox::GetColumnWidth() noexcept
{
	return m_ColumnWidth;
}

void ListBox::SetColumnWidth(const size_t& width) noexcept
{
	m_ColumnWidth = width;
	Update();
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