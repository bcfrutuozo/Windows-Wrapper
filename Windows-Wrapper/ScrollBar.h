#pragma once

#include "Control.h"

class ScrollableControl;

class ScrollBar : public Control
{
	friend class ScrollableControl;

protected:

	ScrollableControl* Owner;
	int Scrolling;
	int MaximumValue;

	void Draw(const Graphics& graphics, Drawing::Rectangle rectangle) override;

	ScrollBar(ScrollableControl* parent, int width, int height, int x, int y);

public:

	virtual ~ScrollBar();

	void Initialize() override;
	int GetScrolling() const noexcept;

	void SetMaximumValue(int maxValue) noexcept;
};