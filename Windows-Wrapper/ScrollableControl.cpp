#include "ScrollableControl.h"

void ScrollableControl::SetMinimumItemWidth(const size_t& weight) noexcept
{
}

ScrollableControl::ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	HorizontalScrollBar(this, m_Size.Width - m_Margin.Left - m_Margin.Right, GetSystemMetrics(SM_CYHSCROLL), m_Margin.Left, m_Size.Height - GetSystemMetrics(SM_CYHSCROLL) - m_Margin.Bottom),
	VerticalScrollBar(this, GetSystemMetrics(SM_CXVSCROLL), m_Size.Height - m_Margin.Top - m_Margin.Bottom, m_Size.Width - GetSystemMetrics(SM_CXVSCROLL) - m_Margin.Right, m_Margin.Top),
	m_ItemWidth(0),
	m_ItemHeight(0),
	m_IsHorizontalScrollEnabled(false),
	m_IsVerticalScrollEnabled(false),
	m_MinimumItemWidth(0)
{

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
	HorizontalScrollBar.Show();
}

void ScrollableControl::DisableHorizontalScroll() noexcept
{
	HorizontalScrollBar.Hide();
}

bool ScrollableControl::IsVerticalScrollEnabled() const noexcept
{
	return VerticalScrollBar.IsShown();
}

void ScrollableControl::EnableVerticalScroll() noexcept
{
	VerticalScrollBar.Show();
}

void ScrollableControl::DisableVerticalScroll() noexcept
{
	VerticalScrollBar.Hide();
}

size_t ScrollableControl::GetItemWidth() const noexcept
{
	return m_ItemWidth;
}

void ScrollableControl::SetItemWidth(const size_t& width)
{
	if (width < m_MinimumItemWidth)
	{
		throw std::logic_error("Desired width for each column is smaller than a single character.");
	}

	m_ItemWidth = width;
}

size_t ScrollableControl::GetItemHeight() const noexcept
{
	return m_ItemHeight;
}

void ScrollableControl::SetItemHeight(const size_t& height) noexcept
{
	m_ItemHeight = height;
}
