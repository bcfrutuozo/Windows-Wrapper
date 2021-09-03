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
	auto totalSizeVertical = m_RowPosition.back().bottom - m_RowPosition.front().top;
	auto percent = ((totalSizeVertical / m_Size.Height) - 1.0f) * 100.0f;
	for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
	{
		if (y >= it->top + m_VerticalScrolling * percent && y <= it->bottom + m_VerticalScrolling * percent && x >= it->left + m_HorizontalScrolling && x <= it->right + m_HorizontalScrolling)
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

	SelectObject(ps.hdc, hFont);

	// Example test draw with the desired font to calculate each ListBox item size
	
	const char* verifier = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	GetTextExtentPoint32(ps.hdc, verifier, 27, &m_SingleSize);

	m_VerticalScrollingUnit = m_SingleSize.cy;

	const auto& dataSource = GetDataSource();
	m_RowPosition.clear();
	if (m_RowPosition.size() < dataSource->GetCount())
	{
		m_RowPosition.resize(dataSource->GetCount());
	}

	// Drawable block inside ListBox
	RECT r(m_Margin.Left, m_Margin.Top, m_Size.Width - m_Margin.Right, m_Size.Height - m_Margin.Bottom);
	if ((static_cast<unsigned long long>(r.bottom) - r.top) < (m_SingleSize .cy * dataSource->GetCount()))
	{
		m_IsVerticalScrollVisible = true;
	}
	else
	{
		m_IsVerticalScrollVisible = false;
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
			RECT cr;
			CopyRect(&cr, &r);
			cr.top = (m_SingleSize.cy * i) - m_VerticalScrolling;
			cr.bottom = (cr.top + m_SingleSize.cy);
			m_RowPosition[i] = cr;

			std::ostringstream oss;
			oss << "Rectangle " << i << " X: " << cr.top << " | Y: " << cr.bottom << std::endl;
			printf_s(oss.str().c_str());

			if (m_SelectedIndex == i)
			{
				SetBkColor(ps.hdc, RGB(0, 120, 215));
				SetTextColor(ps.hdc, RGB(255, 255, 255));
			}
			else
			{
				SetBkColor(ps.hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
				SetTextColor(ps.hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			}

			DrawText(ps.hdc, (*dataSource)[i]->Value.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
			DrawText(ps.hdc, Text.c_str(), static_cast<int>((*dataSource)[i]->Value.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
		}
	}

	auto totalSizeVertical = m_RowPosition.back().bottom - m_RowPosition.front().top;
	m_VerticalScrollingUnit = ((static_cast<float>(m_Size.Height) / static_cast<float>(totalSizeVertical))) * 100.0f;
	m_VerticalScrollPaging = m_SingleSize.cy;

	HandleMessageForwarder(hwnd, WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_RowPosition.back().right - m_RowPosition.back().left, m_VerticalScrollPaging));

	if (ShowScrollBar(hwnd, SB_VERT, m_IsVerticalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	EndPaint(hwnd, &ps);
}

ListBox::ListBox(Control* parent, int width, int height, int x, int y)
	:
	ListControl(parent, "", width, height, x, y),
	m_IsMultiColumn(false),
	m_IsHorizontalScrollVisible(false),
	m_IsVerticalScrollVisible(false),
	m_IsScrollAlwaysVisible(false),
	m_SelectionMode(SelectionMode::Single),
	m_DockStyle(DockStyle::None),
	m_BorderStyle(BorderStyle::Fixed3D),
	m_SingleSize({0})
{
	if (ShowScrollBar(static_cast<HWND>(Handle.ToPointer()), SB_HORZ, m_IsHorizontalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	if (ShowScrollBar(static_cast<HWND>(Handle.ToPointer()), SB_VERT, m_IsVerticalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

ListBox::~ListBox()
{

}