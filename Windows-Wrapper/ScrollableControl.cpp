#include "ScrollableControl.h"
#include "ScrollBar.h"

LPRECT const ScrollableControl::GetDrawableArea() noexcept
{
	return &m_DrawableArea;
}

LPRECT const ScrollableControl::ResetDrawableArea() noexcept
{
	auto size = GetSize();
	m_DrawableArea.left = m_DrawableArea.top = 0;
	m_DrawableArea.right = size.Width;
	m_DrawableArea.bottom = size.Height;
	return &m_DrawableArea;
}

void ScrollableControl::SetMinimumItemWidth(const int& weight) noexcept
{
	m_MinimumItemWidth = weight;
}

ScrollableControl::ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	HorizontalScrollBar(this, GetSize().Width - m_Margin.Left - m_Margin.Right, GetSystemMetrics(SM_CYHSCROLL) + 1, m_Margin.Left, GetSize().Height - (GetSystemMetrics(SM_CYHSCROLL) + 1) - m_Margin.Bottom),
	VerticalScrollBar(this, GetSystemMetrics(SM_CXVSCROLL), GetSize().Height - m_Margin.Top - m_Margin.Bottom, GetSize().Width - GetSystemMetrics(SM_CXVSCROLL) - m_Margin.Right, m_Margin.Top),
	m_ItemWidth(0),
	m_ItemHeight(0),
	m_IsHorizontalScrollEnabled(false),
	m_IsVerticalScrollEnabled(false),
	m_MinimumItemWidth(0)
{
	ResetDrawableArea();
}

ScrollableControl::~ScrollableControl()
{

}

bool ScrollableControl::IsHorizontalScrollEnabled() const noexcept
{
	return HorizontalScrollBar.IsShown();
}

void ScrollableControl::EnableHorizontalScroll() noexcept
{
	if (!IsHorizontalScrollEnabled())
	{
		HorizontalScrollBar.Show();
	}
}

void ScrollableControl::DisableHorizontalScroll() noexcept
{
	if (IsHorizontalScrollEnabled())
	{
		HorizontalScrollBar.Hide();
	}
}

bool ScrollableControl::IsVerticalScrollEnabled() const noexcept
{
	return VerticalScrollBar.IsShown();
}

void ScrollableControl::EnableVerticalScroll() noexcept
{
	if (!VerticalScrollBar.IsShown())
	{
		VerticalScrollBar.Show();
	}
}

void ScrollableControl::DisableVerticalScroll() noexcept
{
	if (VerticalScrollBar.IsShown())
	{
		VerticalScrollBar.Hide();
	}
}

int ScrollableControl::GetItemWidth() const noexcept
{
	return m_ItemWidth;
}

void ScrollableControl::SetItemWidth(const int& width)
{
	if (width < m_MinimumItemWidth)
	{
		throw std::logic_error("Desired width for each column is smaller than a single character.");
	}

	m_ItemWidth = width;
}

int ScrollableControl::GetItemHeight() const noexcept
{
	return m_ItemHeight;
}

void ScrollableControl::SetItemHeight(const int& height) noexcept
{
	m_ItemHeight = height;
}

int ScrollableControl::GetHorizontalPage() const noexcept
{
	return GetSize().Width / GetItemWidth();
}

int ScrollableControl::GetVerticalPage() const noexcept
{
	return GetSize().Height / GetItemHeight();
}
