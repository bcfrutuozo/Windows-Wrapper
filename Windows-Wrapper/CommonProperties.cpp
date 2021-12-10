#include "CommonProperties.h"
#include "LayoutUtils.h"

void CommonProperties::UpdateSpecifiedBounds(IArrangedElement* const element, int x, int y, int width, int height, BoundsSpecified specified)
{
	Drawing::Rectangle originalBounds = CommonProperties::GetSpecifiedBounds(element);
	bool xChangedButNotSpecified = ((specified & BoundsSpecified::X) == BoundsSpecified::None) & x != originalBounds.X;
	bool yChangedButNotSpecified = ((specified & BoundsSpecified::Y) == BoundsSpecified::None) & y != originalBounds.Y;
	bool wChangedButNotSpecified = ((specified & BoundsSpecified::Width) == BoundsSpecified::None) & width != originalBounds.Width;
	bool hChangedButNotSpecified = ((specified & BoundsSpecified::Height) == BoundsSpecified::None) & height != originalBounds.Height;

	if(xChangedButNotSpecified | yChangedButNotSpecified | wChangedButNotSpecified | hChangedButNotSpecified)
	{
		if(!xChangedButNotSpecified) originalBounds.X = x;
		if(!yChangedButNotSpecified) originalBounds.Y = y;
		if(!wChangedButNotSpecified) originalBounds.Width = width;
		if(!hChangedButNotSpecified) originalBounds.Height = height;

		element->SetRectangle(originalBounds);
	}
	else
	{
		//element->SetRectangle(LayoutUtils::MaxRectangle);
	}
}

Drawing::Rectangle CommonProperties::GetSpecifiedBounds(IArrangedElement* const element)
{
	return element->GetRectangle();
}
