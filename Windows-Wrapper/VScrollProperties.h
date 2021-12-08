#pragma once

#include "ScrollProperties.h"

class VScrollProperties : public ScrollProperties
{
public:

	VScrollProperties(ScrollableControl* container);

	int GetPageSize() const noexcept override;
	int GetOrientation() const noexcept override;
	int GetHorizontalDisplayPosition() const noexcept override;
	int GetVerticalDisplayPosition() const noexcept override;
};