#pragma once

#include "ScrollBar.h"

class ScrollableControl;

class VerticalScrollBar : public ScrollBar
{
	friend class ScrollableControl;

private:

	void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept override;
	void OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept override;

public:

	VerticalScrollBar(ScrollableControl* parent, int width, int height, int x, int y);
	virtual ~VerticalScrollBar();
};

