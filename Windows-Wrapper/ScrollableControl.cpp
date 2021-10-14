#include "ScrollableControl.h"
#include "ScrollBar.h"
#include "Exceptions.h"

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

void ScrollableControl::SetItemWidth(int width)
{
	if (width < m_MinimumItemWidth)
	{
		throw ArgumentException("Desired width for each column is smaller than a single character.");
	}

	m_ItemWidth = width;
}

int ScrollableControl::GetItemHeight() const noexcept
{
	return m_ItemHeight;
}

void ScrollableControl::SetItemHeight(int height) noexcept
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
