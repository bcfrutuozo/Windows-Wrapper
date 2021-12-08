#include "VScrollProperties.h"
#include "ScrollableControl.h"

#include <Windows.h>

VScrollProperties::VScrollProperties(ScrollableControl* container)
	:
	ScrollProperties(container)
{}

int VScrollProperties::GetPageSize() const noexcept
{
	return GetParentControl()->GetClientRectangle().GetHeight();
}

int VScrollProperties::GetOrientation() const noexcept
{
	return SB_VERT;
}

int VScrollProperties::GetHorizontalDisplayPosition() const noexcept
{
	return GetParentControl()->GetDisplayRectangle().Left;
}

int VScrollProperties::GetVerticalDisplayPosition() const noexcept
{
	return -GetValue();
}
