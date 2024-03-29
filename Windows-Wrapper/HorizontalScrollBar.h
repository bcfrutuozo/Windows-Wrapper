#pragma once

#include "ScrollBar.h"

class ScrollableControl;

class HorizontalScrollBar : public ScrollBar
{
	friend class ScrollableControl;

private:
	
	void OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) override;
	void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) override;

	HorizontalScrollBar(ScrollableControl* parent, int width, int height, int x, int y);

public:

	virtual ~HorizontalScrollBar();
};

