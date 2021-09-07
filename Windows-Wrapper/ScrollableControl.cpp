#include "ScrollableControl.h"

ScrollableControl::ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	HorizontalScrollBar(this, m_Size.Width - m_Margin.Left - m_Margin.Right, GetSystemMetrics(SM_CYHSCROLL), m_Margin.Left, m_Size.Height - GetSystemMetrics(SM_CYHSCROLL) - m_Margin.Bottom),
	VerticalScrollBar(this, GetSystemMetrics(SM_CXVSCROLL), m_Size.Height - m_Margin.Top - m_Margin.Bottom, m_Size.Width - GetSystemMetrics(SM_CXVSCROLL) - m_Margin.Right, m_Margin.Top),
	m_SingleHorizontalIncrement(0),
	m_SingleVerticalIncrement(0),
	m_IsHorizontalScrollEnabled(false),
	m_IsVerticalScrollEnabled(false)
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

size_t ScrollableControl::GetSingleHorizontalIncrement() const noexcept
{
	return m_SingleHorizontalIncrement;
}

void ScrollableControl::SetSingleHorizontalIncrement(const size_t& increment) noexcept
{
	m_SingleHorizontalIncrement = increment;
}

size_t ScrollableControl::GetSingleVerticalIncrement() const noexcept
{
	return m_SingleVerticalIncrement;
}

void ScrollableControl::SetSingleVerticalIncrement(const size_t& increment) noexcept
{
	m_SingleVerticalIncrement = increment;
}
