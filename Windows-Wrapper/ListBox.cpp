#include "ListBox.h"

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

	WinAPI::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
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
	SIZE s;
	Text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	GetTextExtentPoint32(ps.hdc, Text.c_str(), static_cast<int>(Text.length()), &s);

	const auto& dataSource = GetDataSource();
	m_RowPosition.clear();
	if (m_RowPosition.size() < dataSource->GetCount())
	{
		m_RowPosition.resize(dataSource->GetCount());
	}

	// Drawable block inside ListBox
	RECT r(m_Margin.Left, m_Margin.Top, m_Size.Width - m_Margin.Right, m_Size.Height - m_Margin.Bottom);
	if ((static_cast<unsigned long long>(r.bottom) - r.top) < (s.cy * dataSource->GetCount()))
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
			int columnNumber = (m_Size.Width - m_Margin.Right) / s.cx;
		}
		else
		{
			RECT cr;
			CopyRect(&cr, &r);
			cr.top = s.cy * i;
			cr.bottom = cr.top + s.cy;
			m_RowPosition[i] = cr;

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

			DrawText(ps.hdc, Text.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
			DrawText(ps.hdc, Text.c_str(), static_cast<int>(Text.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
		}
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
	m_BorderStyle(BorderStyle::Fixed3D)
{

}

ListBox::~ListBox()
{

}