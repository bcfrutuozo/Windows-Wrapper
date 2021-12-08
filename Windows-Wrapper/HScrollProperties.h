#pragma once

#include "ScrollProperties.h"

class HScrollProperties : public ScrollProperties
{
public:

	HScrollProperties(ScrollableControl* container);

	int GetPageSize() const noexcept override;
	int GetOrientation() const noexcept override;
	int GetHorizontalDisplayPosition() const noexcept override;
	int GetVerticalDisplayPosition() const noexcept override;
};