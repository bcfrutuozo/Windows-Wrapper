#include "VScrollProperties.h"
#include "ScrollableControl.h"

#include <Windows.h>

VScrollProperties::VScrollProperties(ScrollableControl* container)
	:
	ScrollProperties(container)
{}

int VScrollProperties::GetPageSize() const noexcept
{
	return GetParentControl()->GetClientRectangle().Height;
}

int VScrollProperties::GetOrientation() const noexcept
{
	return SB_VERT;
}

int VScrollProperties::GetHorizontalDisplayPosition() const noexcept
{
	return GetParentControl()->GetDisplayRectangle().X;
}

int VScrollProperties::GetVerticalDisplayPosition() const noexcept
{
	return -GetValue();
}
