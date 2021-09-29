
#include "ListBox.h"
#include "ComboBox.h"

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_DOWN:
	{
		if (m_SelectedIndex < static_cast<int>(GetDataSource().size()) - 1)
		{
			auto drawableArea = GetDrawableArea();

			if (m_SelectionMode == SelectionMode::None) break;

			// If selection is single or is multi but without CTRL or SHIFT pressing
			if (m_SelectionMode == SelectionMode::Single)
			{
				++m_SelectedIndex;
			}
			else
			{
				if ((!(GetKeyState(VK_CONTROL) & 0x8000) && !((GetKeyState(VK_SHIFT) & 0x8000))))
				{
					/*auto p = *(*Items)[++m_SelectedIndex];
					p.Selected = true;
					ListItem* l = new ListItem(p);
					m_SelectedIndices.Add(new int(l->Id));
					m_SelectedItems.Add(l);*/
				}
			}

			if (m_RowPosition[m_SelectedIndex].bottom > drawableArea->bottom && IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].top < drawableArea->top && IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
			}

			if (m_RowPosition[m_SelectedIndex].right > drawableArea->right && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, (m_SelectedIndex / m_RowNumber) - (m_ColumnNumber - 1)), 0);
			}
		}
		else
		{
			if (IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
			}
		}

		break;
	}
	case VK_UP:
	{
		if (m_SelectedIndex > 0)
		{
			auto drawableArea = GetDrawableArea();
			--m_SelectedIndex;

			if (m_RowPosition[m_SelectedIndex].bottom > drawableArea->bottom && IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].top < drawableArea->top && IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex), 0);
			}

			if (m_RowPosition[m_SelectedIndex].left < drawableArea->left && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
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

		if (m_SelectedIndex > m_RowNumber - 1)
		{
			auto drawableArea = GetDrawableArea();
			m_SelectedIndex -= m_RowNumber;

			if (m_RowPosition[m_SelectedIndex].right > drawableArea->right && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].left < drawableArea->left && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
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

		if (m_SelectedIndex < static_cast<int>(GetDataSource().size()) - m_RowNumber)
		{
			auto drawableArea = GetDrawableArea();
			m_SelectedIndex += m_RowNumber;


			if (m_RowPosition[m_SelectedIndex].right > drawableArea->right && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
			}

			if (m_RowPosition[m_SelectedIndex].left < drawableArea->left && IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
			}
		}
		else
		{
			if (IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_SelectedIndex / m_RowNumber), 0);
			}
		}

		break;
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

			if (m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended)
			{
				m_SelectedIndices.resize(itemsNumber);
				m_SelectedItems.resize(itemsNumber);
			}
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
	m_SelectedItem(nullptr)
{

}

ListBox::~ListBox()
{

}

void ListBox::SetSelectedIndex(int index)
{
	switch (m_SelectionMode)
	{
	case SelectionMode::None: throw std::logic_error("Cannot set index with in a ListBox with SelectionMode set as None"); break;
	case SelectionMode::Single:
	{
		m_SelectedIndex = index;

		if (m_SelectedIndex == -1)
		{
			m_SelectedValue = "";
		}
		else
		{
			m_SelectedValue = Items[m_SelectedIndex].Value;
		}
		break;
	}
	case SelectionMode::MultiSimple:
	case SelectionMode::MultiExtended:
	{
		// Clear the list if the passed index is equal -1
		if (index == -1)
		{
			m_SelectedIndices.clear();
		}
		else
		{
			//auto entry = m_SelectedIndices[index];
			// Select this item while keeping any previously selected items selected.
			/*m_SelectedIndices[index] = (&(*Items)[index]->Id);
			m_SelectedItems[index] = (*Items)[index];*/
			Dispatch("OnSelectedIndexChanged", &ArgsDefault);

		}
	}
	}

	Update();
}

void ListBox::SetSelectedValue(const ListItem& item)
{
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
		throw std::invalid_argument("ListItem does not exist");
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
	if (m_SelectionMode == SelectionMode::Single)
	{
		throw std::logic_error("Not supported exception");
	}

	// Clear the SelectedIndices before to add it already ordered
	m_SelectedIndices.clear();

	for (size_t i = 0; i < Items.size(); ++i)
	{
		/*m_SelectedIndices[i] = &(*Items)[i]->Id;
		m_SelectedItems[i] = (*Items)[i];*/
	}
}