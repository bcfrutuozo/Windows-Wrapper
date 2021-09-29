
#include "ListBox.h"
#include "ComboBox.h"

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	if (m_SelectionMode != SelectionMode::None)
	{
		if (m_SelectionMode == SelectionMode::Single || m_SelectionMode == SelectionMode::MultiSimple)
		{
			switch (vk)
			{
			case VK_DOWN:
			{
				if (m_Tabulation < static_cast<int>(GetDataSource().size()) - 1)
				{
					Items[m_Tabulation++].Tabulated = false;
					Items[m_Tabulation].Tabulated = true;

					if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;

					auto drawableArea = GetDrawableArea();

					if (m_RowPosition[m_Tabulation].bottom > drawableArea->bottom && IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation - m_TotalItemsInDrawableArea + 1), 0);
					}

					if (m_RowPosition[m_Tabulation].top < drawableArea->top && IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
					}

					if (m_RowPosition[m_Tabulation].right > drawableArea->right && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, (m_Tabulation / m_RowNumber) - (m_ColumnNumber - 1)), 0);
					}
				}
				else
				{
					if (IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
					}
				}

				break;
			}
			case VK_UP:
			{
				if (m_Tabulation > 0)
				{
					Items[m_Tabulation--].Tabulated = false;
					Items[m_Tabulation].Tabulated = true;

					if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;

					auto drawableArea = GetDrawableArea();

					if (m_RowPosition[m_Tabulation].bottom > drawableArea->bottom && IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation - m_TotalItemsInDrawableArea + 1), 0);
					}

					if (m_RowPosition[m_Tabulation].top < drawableArea->top && IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
					}

					if (m_RowPosition[m_Tabulation].left < drawableArea->left && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
					}
				}
				else
				{
					if (IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
					}
				}

				break;
			}
			case VK_LEFT:
			{
				if (!m_IsMultiColumn)
				{
					break;
				}

				if (m_Tabulation > m_RowNumber - 1)
				{
					auto drawableArea = GetDrawableArea();

					Items[m_Tabulation].Tabulated = false;
					m_Tabulation -= m_RowNumber;
					Items[m_Tabulation].Tabulated = true;

					if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;

					if (m_RowPosition[m_Tabulation].right > drawableArea->right && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
					}

					if (m_RowPosition[m_Tabulation].left < drawableArea->left && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
					}
				}
				else
				{
					if (IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
					}
				}

				break;
			}
			case VK_RIGHT:
			{
				if (!m_IsMultiColumn)
				{
					break;
				}

				if (m_Tabulation < static_cast<int>(GetDataSource().size()) - m_RowNumber)
				{
					auto drawableArea = GetDrawableArea();

					Items[m_Tabulation].Tabulated = false;
					m_Tabulation += m_RowNumber;
					Items[m_Tabulation].Tabulated = true;

					if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;

					if (m_RowPosition[m_Tabulation].right > drawableArea->right && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
					}

					if (m_RowPosition[m_Tabulation].left < drawableArea->left && IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
					}
				}
				else
				{
					if (IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
					}
				}

				break;
			}
			case VK_SPACE:
			{
				if (m_SelectionMode == SelectionMode::MultiSimple)
				{
					SetSelectedIndex(m_Tabulation);
				}

				break;
			}
			case VK_ESCAPE:
			{
				ClearSelected();
				break;
			}
			case 'A':
			{
				if (0x8000 & GetKeyState(VK_CONTROL))
				{
					if (m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended)
					{
						SelectAll();
					}
				}
				break;
			}
			}
		}
		
	}

	Update();
	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void ListBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	int i = 0;

	const auto drawableArea = GetDrawableArea();
	if (y >= drawableArea->top && y <= drawableArea->bottom && x >= drawableArea->left && x <= drawableArea->right)
	{
		for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
		{
			if (y >= it->top && y <= it->bottom && x >= it->left && x <= it->right)
			{
				m_Tabulation = i;
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
	Draw(hwnd, ps.hdc);
	EndPaint(hwnd, &ps);
}

void ListBox::CalculateListBoxParameters(HWND hwnd, HDC& hdc)
{
	// Example test draw with the desired font to calculate each ListBox item size
	SIZE m_SingleSize;
	const char* verifier = "A";
	GetTextExtentPoint32(hdc, verifier, 2, &m_SingleSize);
	SetMinimumItemWidth(m_SingleSize.cx);

	m_BorderSize = 0;
	switch (m_BorderStyle)
	{
	case BorderStyle::FixedSingle: m_BorderSize = 1; break;
	case BorderStyle::Fixed3D: m_BorderSize = 2; break;
	default: break;
	}

	// Drawable block inside ListBox
	auto drawableArea = ResetDrawableArea();
	drawableArea->left += m_Margin.Left + m_BorderSize;
	drawableArea->top += m_Margin.Top + m_BorderSize;
	drawableArea->right -= m_Margin.Right + m_BorderSize;
	drawableArea->bottom -= m_Margin.Bottom + m_BorderSize;

	SetItemWidth(m_IsMultiColumn ? m_ColumnWidth : static_cast<int>(drawableArea->right - drawableArea->left));
	SetItemHeight(m_SingleSize.cy);

	const auto& dataSource = GetDataSource();

	int itemsNumber = static_cast<int>(dataSource.size());

	if (m_IsRebinding)
	{
		// This block will only be executed once after resize
		m_RowPosition.clear();

		if (m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended)
		{
			m_SelectedIndices.clear();
			m_SelectedItems.clear();
		}

		if (static_cast<int>(m_RowPosition.size()) < itemsNumber)
		{
			m_RowPosition.resize(itemsNumber);

			/*if (m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended)
			{
				m_SelectedIndices.resize(itemsNumber);
				m_SelectedItems.resize(itemsNumber);
			}*/
		}
	}

	// Reset the amount of items in drawable area for recalculation
	m_TotalItemsInDrawableArea = 0;

	int newWidth = 0;
	int newHeight = 0;
	int oldWidth = static_cast<int>(drawableArea->right) - static_cast<int>(drawableArea->left);
	int oldHeight = static_cast<int>(drawableArea->bottom) - static_cast<int>(drawableArea->top);

	m_RowNumber = !m_IsMultiColumn ? itemsNumber : (oldHeight) / GetItemHeight();
	m_ColumnNumber = !m_IsMultiColumn ? 1 : (oldWidth) / GetItemWidth();

	if (m_IsMultiColumn)
	{
		if (m_RowNumber * m_ColumnNumber > itemsNumber)
		{
			HorizontalScrollBar.SetMaximumValue(0);
			HorizontalScrollBar.Hide();
		}
		else
		{
			m_RowNumber = m_ColumnNumber = 0;
			while (newWidth < oldWidth)
			{
				newWidth += GetItemWidth();
				++m_ColumnNumber;
				++m_TotalItemsInDrawableArea;
			}

			while (newHeight < oldHeight)
			{
				newHeight += GetItemHeight();
				++m_RowNumber;
			}

			// Recalculate drawable area to check if HorizontalScrollBar is needed

			drawableArea->right = newWidth + m_Margin.Left + (m_BorderSize * 2);
			drawableArea->bottom = newHeight + m_Margin.Top + (m_BorderSize * 2);

			if (m_RowNumber * m_ColumnNumber > itemsNumber)
			{
				HorizontalScrollBar.SetMaximumValue(0);
				HorizontalScrollBar.Hide();
				newWidth += m_Margin.Left + m_Margin.Right + (m_BorderSize * 2);
				oldHeight = m_Size.Width;
				m_Size.Width = newWidth;
				Resize(newWidth, m_Size.Height);
			}
			else
			{
				auto hLoc = HorizontalScrollBar.GetLocation();
				auto hSize = HorizontalScrollBar.GetSize();
				hSize.Width += (newWidth - oldWidth);
				newWidth += m_Margin.Left + m_Margin.Right + (m_BorderSize * 2);
				oldWidth = m_Size.Width;

				newHeight += m_Margin.Top + m_Margin.Bottom + (m_BorderSize * 2) + hSize.Height;
				oldHeight = m_Size.Height;
				m_Size.Height = newHeight;
				m_Size.Width = newWidth;
				Resize(newWidth, newHeight);
				HorizontalScrollBar.SetLocation(hLoc.X, hLoc.Y + (newHeight - oldHeight));
				HorizontalScrollBar.Resize(hSize);
				HorizontalScrollBar.SetMaximumValue(static_cast<int>(std::ceil(static_cast<float>(itemsNumber) / (m_RowNumber))));
				HorizontalScrollBar.Show();
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_TotalItemsInDrawableArea, 0));
			}
		}
	}
	else
	{
		if (oldHeight > (GetItemHeight() * itemsNumber))
		{
			VerticalScrollBar.SetMaximumValue(0);
			VerticalScrollBar.Hide();
		}
		else
		{
			while (newHeight < oldHeight)
			{
				newHeight += GetItemHeight();
			}

			drawableArea->right -= VerticalScrollBar.GetSize().Width;

			// Recalculate the new item width size
			SetItemWidth(m_IsMultiColumn ? m_ColumnWidth : drawableArea->right - drawableArea->left);

			drawableArea->bottom = newHeight + m_Margin.Top + (m_BorderSize * 2);
			newHeight += m_Margin.Top + m_Margin.Bottom + (m_BorderSize * 2);
			int oldHeightY = m_Size.Height;
			m_Size.Height = newHeight;
			auto vSize = VerticalScrollBar.GetSize();
			vSize.Height += (newHeight - oldHeightY);
			VerticalScrollBar.Resize(vSize);
			VerticalScrollBar.SetMaximumValue(itemsNumber);
			VerticalScrollBar.Show();
			Resize(m_Size.Width, newHeight + 1);

			int max = 0;
			while (max < newHeight)
			{
				max += GetItemHeight();

				if (max > newHeight)
				{
					break;
				}

				++m_TotalItemsInDrawableArea;
			}

			HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(0, m_TotalItemsInDrawableArea));
		}
	}

	m_IsRebinding = false;
	m_IsFormatChanged = false;
}

void ListBox::Draw(HWND hwnd, HDC& hdc)
{
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

	SelectObject(hdc, hFont);

	HDC hdcMem;
	HBITMAP hbmMem;
	HBITMAP hbmOld;

	auto drawableArea = GetDrawableArea();
	if (m_IsRebinding || m_IsFormatChanged)
	{
		CalculateListBoxParameters(hwnd, hdc);
	}

	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc, m_Size.Width, m_Size.Height);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	SelectObject(hdcMem, hFont);

	RECT r;
	GetClientRect(hwnd, &r);

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

	const auto& dataSource = GetDataSource();

	/* Logic to draw only visible entries. However, it's much harder to track the selected index
	because it position cannot be tracked to be handled.

	for (auto [i, r, c, a] =
		std::tuple{
		IsMultiColumn() ? HorizontalScrollBar.GetScrolling() * m_RowNumber : VerticalScrollBar.GetScrolling() * m_ColumnNumber,
		0,
		0,
		0 };
		a < m_RowNumber * m_ColumnNumber;
		++i, ++r, ++a)
	{
		if (i >= dataSource->GetCount())
		{
			break;
		}
		RECT cr;
		CopyRect(&cr, drawableArea);
		cr.top = drawableArea->top + (GetItemHeight() * r);
		cr.bottom = (cr.top + GetItemHeight());
		cr.left = drawableArea->left + (GetItemWidth() * c);
		cr.right = cr.left + GetItemWidth() - m_Margin.Right;
		if (r == m_RowNumber - 1)
		{
			r = -1;
			++c;
		}
		if (m_SelectedIndex == i)
		{
			SetBkColor(hdcMem, RGB(0, 120, 215));
			SetTextColor(hdcMem, RGB(255, 255, 255));
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
	}*/

	for (auto [i, rw, c] = std::tuple{ 0, 0, 0 }; i < static_cast<int>(dataSource.size()); ++i, ++rw)
	{
		RECT cr;
		CopyRect(&cr, drawableArea);

		cr.top = drawableArea->top - (GetItemHeight() * VerticalScrollBar.GetScrolling()) + (GetItemHeight() * rw);
		cr.bottom = (cr.top + GetItemHeight());
		cr.left = drawableArea->left - (HorizontalScrollBar.GetScrolling() * GetItemWidth()) + (GetItemWidth() * c);
		cr.right = cr.left + GetItemWidth() - m_Margin.Right;

		if (rw == m_RowNumber - 1)
		{
			rw = -1;
			++c;
		}

		m_RowPosition[i] = cr;

		if (cr.left >= drawableArea->left &&
			cr.top >= drawableArea->top &&
			cr.right <= drawableArea->right &&
			cr.bottom <= drawableArea->bottom)
		{
			if (dataSource[i].Tabulated && IsTabSelected() && m_SelectionMode == SelectionMode::MultiSimple)
			{
				HPEN pen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
				HGDIOBJ old_pen = SelectObject(hdcMem, pen);
				SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetG()));
				Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);

				SelectObject(hdcMem, old_pen);
				DeleteObject(old_pen);
				SelectObject(hdcMem, pen);
				DeleteObject(pen);
			}

			// To draw tabulation dot
			cr.left += 1;
			cr.top += 1;
			cr.right -= 1;
			cr.bottom -= 1;

			HBRUSH brush = 0;

			if((m_SelectionMode == SelectionMode::Single && m_SelectedIndex == i) || 
				(m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended) && std::find(m_SelectedIndices.begin(), m_SelectedIndices.end(), i) != m_SelectedIndices.end())
			{
				SetBkColor(hdcMem, RGB(0, 120, 215));
				SetTextColor(hdcMem, RGB(255, 255, 255));
				brush = CreateSolidBrush(RGB(0, 120, 215));
			}
			else
			{
				SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
				SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
				brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));				
			}

			FillRect(hdcMem, &cr, brush);
			SelectObject(hdcMem, brush);
			DeleteObject(brush);

			DrawText(hdcMem, dataSource[i].Value.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
			DrawText(hdcMem, Text.c_str(), static_cast<int>(dataSource[i].Value.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
		}
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hFont);
	DeleteObject(hFont);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
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
	m_TotalItemsInDrawableArea(0),
	m_ColumnWidth(120),
	m_ColumnNumber(1),
	m_RowNumber(1),
	m_IsFormatChanged(false),
	m_SelectedItem(nullptr),
	m_Tabulation(-1)
{

}

ListBox::~ListBox()
{

}

void ListBox::SetSelectedIndex(int index)
{
	if (Items.size() == 0) return;

	switch (m_SelectionMode)
	{
	case SelectionMode::None: throw ArgumentException("Cannot set index with in a ListBox with SelectionMode set as None"); break;
	case SelectionMode::Single:
	{
		if (index < -1 || index >= Items.size()) throw ArgumentOutOfRangeException("index");

		m_SelectedIndex = index;

		if (m_SelectedIndex == -1)
		{
			m_SelectedValue = "";
		}
		else
		{
			if (Items[m_SelectedIndex].Selected == true)
			{
				Items[m_SelectedIndex].Selected = false;
				m_SelectedValue = -1;
			}
			else
			{
				Items[m_SelectedIndex].Selected = true;
				m_SelectedValue = Items[m_SelectedIndex].Value;
			}
		}
		break;
	}
	case SelectionMode::MultiSimple:
	case SelectionMode::MultiExtended:
	{
		if (index < -1 || index >= Items.size()) throw ArgumentOutOfRangeException("index");

		m_SelectedIndex = index;

		// Clear the list if the passed index is equal -1
		if (m_SelectedIndex == -1)
		{
			for (auto& it : m_SelectedItems)
			{
				it->Selected = false;
			}

			m_SelectedIndices.clear();
			m_SelectedItems.clear();
		}
		else
		{
			if (Items[m_SelectedIndex].Selected == true)
			{
				Items[m_SelectedIndex].Selected = false;
				m_SelectedIndices.erase(std::remove_if(m_SelectedIndices.begin(), m_SelectedIndices.end(), [&](int i) { return i == m_SelectedIndex; }));
				m_SelectedItems.erase(std::remove_if(m_SelectedItems.begin(), m_SelectedItems.end(), [&](ListItem* it) { return it->Equals(&Items[m_SelectedIndex]); }));

				if (m_SelectedItems.size() == 0)
				{
					m_SelectedItem = nullptr;
					m_SelectedValue = "";
				}
				else
				{
					m_SelectedItem = m_SelectedItems.back();
					m_SelectedValue = m_SelectedItems.back()->Value;
				}

				if (m_SelectedIndices.size() == 0) m_SelectedIndex = -1;
				else m_SelectedIndex = m_SelectedIndices.back();
			}
			else
			{
				Items[m_SelectedIndex].Selected = true;
				// Select this item while keeping any previously selected items selected.
				m_SelectedIndices.push_back(Items[m_SelectedIndex].Id);
				m_SelectedItems.push_back(&Items[m_SelectedIndex]);
				m_SelectedValue = Items[m_SelectedIndex].Value;
				m_SelectedIndex = m_SelectedIndex;
			}
		}
	}
	}

	Dispatch("OnSelectedIndexChanged", &ArgsDefault);
	Update();
}

void ListBox::SetSelectedValue(const ListItem& item)
{
	if (Items.size() == 0) return;

	bool err = true;

	for (size_t i = 0; i < Items.size(); ++i)
	{
		if (Items[i].Id == item.Id && Items[i].Value == item.Value)
		{
			SetSelectedIndex(static_cast<int>(i));
			err = false;
			break;
		}
	}

	if (err)
	{
		throw ArgumentOutOfRangeException("The selected value is not in the list of available values");
	}
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
		m_IsFormatChanged = true;
		Update();
	}
}

void ListBox::DisableMultiColumn() noexcept
{
	if (m_IsMultiColumn)
	{
		m_IsMultiColumn = false;
		m_IsFormatChanged = true;
		Update();
	}
}

int ListBox::GetColumnWidth() noexcept
{
	return m_ColumnWidth;
}

void ListBox::SetColumnWidth(const int& width) noexcept
{
	if (m_IsMultiColumn)
	{
		m_ColumnWidth = width;
		Update();
	}
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

SelectionMode ListBox::GetSelectionMode() const noexcept
{
	return m_SelectionMode;
}

void ListBox::SetSelectionMode(SelectionMode mode) noexcept
{
	if (m_SelectionMode == mode)
	{
		return;
	}

	if (m_SelectedIndices.size() > 1)
	{
		m_SelectedIndices.clear();
	}

	m_SelectionMode = mode;
}

void ListBox::SelectAll()
{
	if (Items.size() == 0) return;

	if (m_SelectionMode == SelectionMode::Single)
	{
		throw NotSupportedException("ListBox with Single selection cannot select all elements");
	}

	// Clear the SelectedIndices before to add it already ordered
	ClearSelected();

	for (size_t i = 0; i < Items.size(); ++i)
	{
		SetSelectedIndex(static_cast<int>(i));
	}

	m_SelectedValue = m_SelectedItems.back()->Value;
	m_SelectedIndex = m_SelectedIndices.back();
	Dispatch("OnSelectedIndexChanged", &ArgsDefault);
}

void ListBox::ClearSelected() noexcept
{
	if (Items.size() == 0) return;

	// In case no elements is selected
	if (m_SelectedIndex == -1) return;

	switch (m_SelectionMode)
	{
	case SelectionMode::None: break;
	case SelectionMode::Single:
	{
		Items[m_SelectedIndex].Selected = false;
		break;
	}
	case SelectionMode::MultiSimple:
	case SelectionMode::MultiExtended:
	{
		for (auto& it : m_SelectedItems)
		{
			it->Selected = false;
		}

		m_SelectedIndices.clear();
		m_SelectedItems.clear();
		break;
	}
	}

	m_SelectedIndex = -1;
	m_SelectedValue = "";
	m_SelectedItem = nullptr;
}
