#include "ListBox.h"
#include "ComboBox.h"
#include "Exceptions.h"

void ListBox::PreDraw(Graphics* const graphics)
{
	// Load current font from default PreDraw function
	NativeWindow::PreDraw(graphics);

	if (m_IsRebinding || m_IsFormatChanged)
	{
		// Example test draw with the desired font to calculate each ListBox item size
		const char* verifier = "A";
		auto size = graphics->GetTextSize(verifier, GetFont());
		SetMinimumItemWidth(size.Width);

		m_BorderSize = 0;
		switch (m_BorderStyle)
		{
			case BorderStyle::FixedSingle: m_BorderSize = 1; break;
			case BorderStyle::Fixed3D: m_BorderSize = 2; break;
			default: break;
		}

		// Drawable block inside ListBox
		auto drawableArea = GetDrawableArea();
		drawableArea.Left += m_BorderSize;
		drawableArea.Top += m_BorderSize;
		drawableArea.Right -= m_BorderSize;
		drawableArea.Bottom -= m_BorderSize;

		SetItemWidth(m_IsMultiColumn ? m_ColumnWidth : static_cast<int>(drawableArea.Right - drawableArea.Left));
		SetItemHeight(size.Height);

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
		int oldWidth = static_cast<int>(drawableArea.Right) - static_cast<int>(drawableArea.Left);
		int oldHeight = static_cast<int>(drawableArea.Bottom) - static_cast<int>(drawableArea.Top);

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

				drawableArea.Right = newWidth + m_Margin.Left + (m_BorderSize * 2);
				drawableArea.Bottom = newHeight + m_Margin.Top + (m_BorderSize * 2);

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

				drawableArea.Right -= VerticalScrollBar.GetSize().Width;

				// Recalculate the new item width size
				SetItemWidth(m_IsMultiColumn ? m_ColumnWidth : drawableArea.Right - drawableArea.Left);

				drawableArea.Bottom = newHeight + m_Margin.Top + (m_BorderSize * 2);
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

	NativeWindow::PreDraw(graphics);
}

void ListBox::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{
	Drawing::Rectangle insider = rectangle;

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
				insider = graphics->DrawRectangle(Color::Black(), insider, 1, ChartDashStyle::Solid);
			}
			else
			{
				if (IsMouseOver())
				{
					insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Solid);

				}
				else
				{
					insider = graphics->DrawRectangle(Color(122, 122, 122), insider, 1, ChartDashStyle::Solid);
				}
			}
			break;
		}
		case BorderStyle::Fixed3D:
		{
			insider = graphics->DrawRectangle(Color(130, 135, 144), insider, 1, ChartDashStyle::Solid);
			insider = graphics->DrawRectangle(Color(255, 255, 255), insider, 1, ChartDashStyle::Solid);

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

	auto drawableArea = GetDrawableArea();
	for (auto [i, rw, c] = std::tuple{ 0, 0, 0 }; i < static_cast<int>(dataSource.size()); ++i, ++rw)
	{
		Drawing::Rectangle cr = drawableArea;

		cr.Top = drawableArea.Top - (GetItemHeight() * VerticalScrollBar.GetScrolling()) + (GetItemHeight() * rw);
		cr.Bottom = (cr.Top + GetItemHeight());
		cr.Left = drawableArea.Left - (HorizontalScrollBar.GetScrolling() * GetItemWidth()) + (GetItemWidth() * c);
		cr.Right = cr.Left + GetItemWidth() - m_Margin.Right;

		if (rw == m_RowNumber - 1)
		{
			rw = -1;
			++c;
		}

		m_RowPosition[i] = cr;

		if (cr.Left >= drawableArea.Left &&
			cr.Top >= drawableArea.Top &&
			cr.Right <= drawableArea.Right &&
			cr.Bottom <= drawableArea.Bottom)
		{
			if (dataSource[i].Tabulated && IsTabSelected() && m_SelectionMode == SelectionMode::MultiSimple)
			{
				cr = graphics->DrawRectangle(GetBackgroundColor(), cr, 1, ChartDashStyle::Dot);
			}

			if ((m_SelectionMode == SelectionMode::Single && m_SelectedIndex == i) ||
				(m_SelectionMode == SelectionMode::MultiSimple || m_SelectionMode == SelectionMode::MultiExtended) && std::find(m_SelectedIndices.begin(), m_SelectedIndices.end(), i) != m_SelectedIndices.end())
			{
				graphics->DrawOpaqueText(dataSource[i].Value, HorizontalAlignment::Left, GetFont(), Color::SelectionForeground(), cr, Color::SelectionBackground());
			}
			else
			{
				graphics->DrawTransparentText(dataSource[i].Value, HorizontalAlignment::Left, GetFont(), GetForeColor(), cr);
			}
		}
	}
}

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	if (m_SelectionMode != SelectionMode::None)
	{
		if (m_Tabulation == -1)
		{
			m_Tabulation = 0;
			Items[0].Tabulated = true;

			if (m_SelectionMode == SelectionMode::Single || m_SelectionMode == SelectionMode::MultiExtended)
			{
				SetSelectedIndex(0, true);
			}

			if (IsVerticalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
			}
			else if (IsHorizontalScrollEnabled())
			{
				HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
			}
		}
		else
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
						else if (m_SelectionMode == SelectionMode::MultiExtended)
						{
							if (0x8000 & GetKeyState(VK_SHIFT))
							{
								m_SelectionEnd = m_Tabulation;

								int start = (std::min)(m_SelectionStart, m_SelectionEnd);
								int end = (std::max)(m_SelectionStart, m_SelectionEnd);

								ClearSelected();
								for (; start <= end; ++start)
								{
									SetSelectedIndex(start, true);
								}
							}
							else
							{
								m_SelectionStart = m_SelectionEnd = m_Tabulation;
								ClearSelected();
								SetSelectedIndex(m_Tabulation, true);
							}
						}

						if (IsVerticalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Bottom > drawableArea.Bottom)
							{
								HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation - m_TotalItemsInDrawableArea + 1), 0);
							}

							if (m_RowPosition[m_Tabulation].Top < drawableArea.Top)
							{
								HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
							}
						}
						else if (IsHorizontalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Right > drawableArea.Right)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, (m_Tabulation / m_RowNumber) - (m_ColumnNumber - 1)), 0);
							}

							if (m_RowPosition[m_Tabulation].Left < drawableArea.Left)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
							}
						}
					}
					else
					{
						if (IsVerticalScrollEnabled())
						{
							HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
						}
						else if (IsHorizontalScrollEnabled())
						{
							HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
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
						else if (m_SelectionMode == SelectionMode::MultiExtended)
						{
							if (0x8000 & GetKeyState(VK_SHIFT))
							{
								m_SelectionEnd = m_Tabulation;

								int start = (std::min)(m_SelectionStart, m_SelectionEnd);
								int end = (std::max)(m_SelectionStart, m_SelectionEnd);

								ClearSelected();
								for (; start <= end; ++start)
								{
									SetSelectedIndex(start, true);
								}
							}
							else
							{
								m_SelectionStart = m_SelectionEnd = m_Tabulation;
								ClearSelected();
								SetSelectedIndex(m_Tabulation, true);
							}
						}

						if (IsVerticalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Bottom > drawableArea.Bottom)
							{
								HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation - m_TotalItemsInDrawableArea + 1), 0);
							}

							if (m_RowPosition[m_Tabulation].Top < drawableArea.Top)
							{
								HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation), 0);
							}
						}
						else if (IsHorizontalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Right > drawableArea.Right)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, (m_Tabulation / m_RowNumber) - (m_ColumnNumber - 1)), 0);
							}

							if (m_RowPosition[m_Tabulation].Left < drawableArea.Left)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
							}
						}
					}
					else
					{
						if (IsVerticalScrollEnabled())
						{
							HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
						}
						else if (IsHorizontalScrollEnabled())
						{
							HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, 0), 0);
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
						Items[m_Tabulation].Tabulated = false;
						m_Tabulation -= m_RowNumber;
						Items[m_Tabulation].Tabulated = true;

						if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;
						else if (m_SelectionMode == SelectionMode::MultiExtended)
						{
							if (0x8000 & GetKeyState(VK_SHIFT))
							{
								m_SelectionEnd = m_Tabulation;

								int start = (std::min)(m_SelectionStart, m_SelectionEnd);
								int end = (std::max)(m_SelectionStart, m_SelectionEnd);

								ClearSelected();
								for (; start <= end; ++start)
								{
									SetSelectedIndex(start, true);
								}
							}
							else
							{
								m_SelectionStart = m_SelectionEnd = m_Tabulation;
								ClearSelected();
								SetSelectedIndex(m_Tabulation, true);
							}
						}

						if (IsHorizontalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Right > drawableArea.Right)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
							}

							if (m_RowPosition[m_Tabulation].Left < drawableArea.Left)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
							}
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
						Items[m_Tabulation].Tabulated = false;
						m_Tabulation += m_RowNumber;
						Items[m_Tabulation].Tabulated = true;


						if (m_SelectionMode == SelectionMode::Single) m_SelectedIndex = m_Tabulation;
						else if (m_SelectionMode == SelectionMode::MultiExtended)
						{
							if (0x8000 & GetKeyState(VK_SHIFT))
							{
								m_SelectionEnd = m_Tabulation;

								int start = (std::min)(m_SelectionStart, m_SelectionEnd);
								int end = (std::max)(m_SelectionStart, m_SelectionEnd);

								ClearSelected();
								for (; start <= end; ++start)
								{
									SetSelectedIndex(start, true);
								}
							}
							else
							{
								m_SelectionStart = m_SelectionEnd = m_Tabulation;
								ClearSelected();
								SetSelectedIndex(m_Tabulation, true);
							}
						}

						if (IsHorizontalScrollEnabled())
						{
							auto drawableArea = GetDrawableArea();
							if (m_RowPosition[m_Tabulation].Right > drawableArea.Right)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber - m_TotalItemsInDrawableArea + 1), 0);
							}

							if (m_RowPosition[m_Tabulation].Left < drawableArea.Left)
							{
								HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, m_Tabulation / m_RowNumber), 0);
							}
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
						SetSelectedIndex(m_Tabulation, true);
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
				case VK_NEXT:
				{
					if (IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
					}
					else if (IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), 0);
					}

					break;
				}
				case VK_PRIOR:
				{
					if (IsVerticalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(VerticalScrollBar.Handle.ToPointer()), WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
					}
					else if (IsHorizontalScrollEnabled())
					{
						HandleMessageForwarder(static_cast<HWND>(HorizontalScrollBar.Handle.ToPointer()), WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, 0), 0);
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
	if (m_SelectionMode != SelectionMode::None)
	{

		int i = 0;

		const auto drawableArea = GetDrawableArea();
		if (y >= drawableArea.Top && y <= drawableArea.Bottom && x >= drawableArea.Left && x <= drawableArea.Right)
		{
			for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
			{
				if (y >= it->Top && y <= it->Bottom && x >= it->Left && x <= it->Right)
				{
					if (m_Tabulation > -1)
					{
						int oldTab = m_Tabulation;
						Items[oldTab].Tabulated = false;
					}

					m_Tabulation = i;
					Items[m_Tabulation].Tabulated = true;

					if (m_SelectionMode == SelectionMode::MultiExtended)
					{
						if (keyFlags & MK_SHIFT)
						{
							m_SelectionEnd = i;

							int start = (std::min)(m_SelectionStart, m_SelectionEnd);
							int end = (std::max)(m_SelectionStart, m_SelectionEnd);

							ClearSelected();
							for (; start <= end; ++start)
							{
								SetSelectedIndex(start, true);
							}
						}
						else
						{
							m_SelectionStart = m_SelectionEnd = i;

							if ((keyFlags & MK_CONTROL))
							{
								SetSelectedIndex(i, !Items[i].Selected);
							}
							else
							{
								ClearSelected();
								SetSelectedIndex(i, true);

							}
						}
					}
					else
					{
						SetSelectedIndex(i, true);
					}

					break;
				}
			}
		}
	}

	Control::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
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
	m_Tabulation(-1),
	m_SelectionStart(-1),
	m_SelectionEnd(-1)
{

}

ListBox::~ListBox()
{

}

void ListBox::SetSelectedIndex(int index, bool value)
{
	if (Items.size() == 0) return;

	switch (m_SelectionMode)
	{
		case SelectionMode::None: throw ArgumentException("Cannot set index with in a ListBox with SelectionMode set as None"); break;
		case SelectionMode::Single:
		{
			if (index < -1 || index >= Items.size()) throw ArgumentOutOfRangeException("index");

			if (index == -1)
			{
				m_SelectedValue = "";
			}
			else
			{
				if (!value)
				{
					if (!Items[index].Selected) break;

					Items[index].Selected = false;
					m_SelectedValue = -1;
				}
				else
				{
					Items[index].Selected = true;
					m_SelectedValue = Items[index].Value;

					// Clear old selection
					Items[index].Selected = false;
					m_SelectedIndex = index;
				}
			}
			break;
		}
		case SelectionMode::MultiSimple:
		case SelectionMode::MultiExtended:
		{
			if (index < -1 || index >= Items.size()) throw ArgumentOutOfRangeException("index");

			// Clear the list if the passed index is equal -1
			if (index == -1)
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
				if (Items[index].Selected && !value)
				{
					Items[index].Selected = false;
					m_SelectedIndices.erase(std::remove_if(m_SelectedIndices.begin(), m_SelectedIndices.end(), [&](int i) { return i == index; }));
					m_SelectedItems.erase(std::remove_if(m_SelectedItems.begin(), m_SelectedItems.end(), [&](ListItem* it) { return it->Equals(&Items[index]); }));

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
				else if (!Items[index].Selected && value)
				{
					Items[index].Selected = true;
					// Select this item while keeping any previously selected items selected.
					m_SelectedIndices.push_back(index);
					m_SelectedItems.push_back(&Items[index]);
					m_SelectedValue = Items[index].Value;
					m_SelectedIndex = index;
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
			SetSelectedIndex(static_cast<int>(i), true);
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
		SetSelectedIndex(static_cast<int>(i), true);
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
