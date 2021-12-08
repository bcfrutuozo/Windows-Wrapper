#pragma once

#include "Control.h"
#include "HScrollProperties.h"
#include "VScrollProperties.h"
#include "Point.h"
#include "Size.h"

class ScrollableControl : public Control
{
	friend class HScrollProperties;
	friend class VScrollProperties;
private:

	Size m_UserAutoScrollMinSize;
	Drawing::Rectangle m_DisplayRectangle;
	Size m_ScrollMargin;
	Size m_RequestedScrollMargin;
	Point m_ScrollPosition;
	int m_ScrollState;

	HScrollProperties HorizontalScroll;
	VScrollProperties VerticalScroll;

	void UpdateFullDrag();

protected:

	static constexpr int ScrollStateAutoScrolling = 0x0001;
	static constexpr int ScrollStateHScrollVisible = 0x0002;
	static constexpr int ScrollStateVScrollVisible = 0x0004;
	static constexpr int ScrollStateUserHasScrolled = 0x0008;
	static constexpr int ScrollStateFullDrag = 0x0010;

	ScrollableControl(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~ScrollableControl();
	
	Drawing::Rectangle GetDisplayRectangle();

	bool GetScrollState(int bit) const noexcept;
	void SetScrollState(int bit, bool value);

	bool GetHScroll() const noexcept;
	void SetHScroll(bool value);
	bool GetVScroll() const noexcept;
	void SetVScroll(bool value);

public:

	virtual bool HasAutoScroll();
	virtual void SetAutoScroll(bool value);
};