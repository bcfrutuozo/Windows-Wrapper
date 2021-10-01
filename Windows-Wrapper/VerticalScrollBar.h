#pragma once

#include "ScrollBar.h"

class ScrollableControl;

class VerticalScrollBar : public ScrollBar
{
	friend class ScrollableControl;

private:

	void Draw(const Graphics& graphics, Drawing::Rectangle rectangle);
	void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) override;
	void OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) override;

public:

	VerticalScrollBar(ScrollableControl* parent, int width, int height, int x, int y);
	virtual ~VerticalScrollBar();
};

