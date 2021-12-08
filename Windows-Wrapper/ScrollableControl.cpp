#include "ScrollableControl.h"
#include "ScrollBar.h"
#include "Exceptions.h"

void ScrollableControl::UpdateFullDrag()
{
	SetScrollState(ScrollStateFullDrag, SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, nullptr, 0));
}

ScrollableControl::ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	m_UserAutoScrollMinSize(Size::Empty()),
	m_DisplayRectangle(Drawing::Rectangle::Empty()),
	m_ScrollMargin(Size::Empty()),
	m_RequestedScrollMargin(Size::Empty()),
	m_ScrollPosition(Point::Empty()),
	HorizontalScroll(this),
	VerticalScroll(this),
	m_ScrollState(0)
{

}

ScrollableControl::~ScrollableControl()
{

}

Drawing::Rectangle ScrollableControl::GetDisplayRectangle()
{
	auto rect = GetClientRectangle();
	if(!m_DisplayRectangle.IsEmpty())
	{
		rect.Left = m_DisplayRectangle.Left;
		rect.Top = m_DisplayRectangle.Top;

		if(GetHScroll()) rect.Right = m_DisplayRectangle.Right;
		if(GetVScroll()) rect.Bottom = m_DisplayRectangle.Bottom;
	}
	//rect.Deflate()

	return rect;
}

bool ScrollableControl::GetScrollState(int bit) const noexcept
{
	return (bit & m_ScrollState) == bit;
}

void ScrollableControl::SetScrollState(int bit, bool value)
{
	if(value) m_ScrollState |= bit;
	else m_ScrollState &= (~bit);
}

bool ScrollableControl::GetHScroll() const noexcept
{
	return GetScrollState(ScrollStateHScrollVisible);
}

void ScrollableControl::SetHScroll(bool value)
{
	SetScrollState(ScrollStateHScrollVisible, value);
}

bool ScrollableControl::GetVScroll() const noexcept
{
	return GetScrollState(ScrollStateVScrollVisible);
}

void ScrollableControl::SetVScroll(bool value)
{
	SetScrollState(ScrollStateVScrollVisible, value);
}

bool ScrollableControl::HasAutoScroll()
{
	return GetScrollState(ScrollStateAutoScrolling);
}

void ScrollableControl::SetAutoScroll(bool value)
{
	if(value) UpdateFullDrag();

	SetScrollState(ScrollStateAutoScrolling, true);
}

