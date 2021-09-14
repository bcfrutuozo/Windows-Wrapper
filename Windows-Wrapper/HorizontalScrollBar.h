#pragma once

#include "ScrollBar.h"

class ScrollableControl;

class HorizontalScrollBar : public ScrollBar
{
private:

	void OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept override;
	void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept override;

public:

	HorizontalScrollBar(ScrollableControl* parent, int width, int height, int x, int y);
	virtual ~HorizontalScrollBar();
};

