#pragma once

#include "Control.h"
#include "ICollection.h"

class ScrollableControl;

class ScrollBar : public Control
{
protected:

	ScrollableControl* Owner;
	bool IsScrollVisible;
	size_t Scrolling;
	size_t MaximumValue;

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	void OnPaint_Impl(HWND hwnd) noexcept override;

public:

	ScrollBar(ScrollableControl* parent, int width, int height, int x, int y);
	virtual ~ScrollBar();

	void Initialize() override;
	void Show() noexcept override;
	void Hide() noexcept override;
	size_t GetScrolling() const noexcept;

	void SetMaximumValue(size_t maxValue) noexcept;
};