#include "HScrollProperties.h"
#include "ScrollableControl.h"

#include <Windows.h>

HScrollProperties::HScrollProperties(ScrollableControl* container)
	:
	ScrollProperties(container)
{}

int HScrollProperties::GetPageSize() const noexcept
{
	return GetParentControl()->GetClientRectangle().Width;
}

int HScrollProperties::GetOrientation() const noexcept
{
	return SB_HORZ;
}

int HScrollProperties::GetHorizontalDisplayPosition() const noexcept
{
	return -GetValue();
}

int HScrollProperties::GetVerticalDisplayPosition() const noexcept
{
	return GetParentControl()->GetDisplayRectangle().Y;
}
