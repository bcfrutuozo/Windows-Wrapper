#pragma once

#include "Control.h"
#include "HScrollProperties.h"
#include "VScrollProperties.h"
#include "Point.h"
#include "Size.h"
#include "ScrollOrientation.h"
#include "DockPaddingEdges.h"

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
	bool m_ResetRTLHScrollValue;
	DockPaddingEdges m_DockPadding;

	HScrollProperties HorizontalScroll;
	VScrollProperties VerticalScroll;

	Drawing::Rectangle GetDisplayRectInternal() noexcept;
	int ScrollThumbPosition(int fnBar);
	void UpdateFullDrag();
	void SyncScrollbars(bool hasAutoScroll);
	void ResetScrollProperties(ScrollProperties* const scrollProperties);

	void WmVScroll(Message& m);
	void WmHScroll(Message& m);
	void WmOnScroll(Message& m, int oldValue, int value, ScrollOrientation scrollOrientation);
	void WmSettingChange(Message& m);

	void OnSetScrollPosition(Object* const sender, EventArgs* const e);

protected:

	static constexpr int ScrollStateAutoScrolling = 0x0001;
	static constexpr int ScrollStateHScrollVisible = 0x0002;
	static constexpr int ScrollStateVScrollVisible = 0x0004;
	static constexpr int ScrollStateUserHasScrolled = 0x0008;
	static constexpr int ScrollStateFullDrag = 0x0010;

	ScrollableControl();
	virtual ~ScrollableControl();
	
	Drawing::Rectangle GetDisplayRectangle();
	bool GetScrollState(int bit) const noexcept;
	void SetScrollState(int bit, bool value);
	bool GetHScroll() const noexcept;
	void SetHScroll(bool value);
	bool GetVScroll() const noexcept;
	void SetVScroll(bool value);

	virtual Point ScrollToControl(Control* activeControl);
	void SetDisplayRectLocation(int x, int y);
	virtual CreateParams* CreateParameters() override;
	void WndProc(Message& m) override;

public:

	virtual bool HasAutoScroll();
	virtual void SetAutoScroll(bool value);

	Size GetAutoScrollMargin() const noexcept;
	void SetAutoScrollMargin(int x, int y);
	void SetAutoScrollMargin(Size s);
	void ScrollControlIntoView(Control* activeControl);
};