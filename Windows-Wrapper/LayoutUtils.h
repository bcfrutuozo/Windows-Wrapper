#pragma once

#include "Padding.h"
#include "Size.h"
#include "Rectangle.h"

class LayoutUtils
{
public:

	static constexpr Drawing::Rectangle MaxRectangle = Drawing::Rectangle(0, 0, INT_MAX, INT_MAX);

	static constexpr Size IntersectSizes(Size a, Size b)
	{
		return Size((std::min)(a.Width, b.Width), (std::min)(a.Height, b.Height));
	}

	static constexpr Size ConvertZeroToUnbounded(Size size)
	{
		if(size.Width == 0) size.Width = INT_MAX;
		if(size.Height == 0) size.Height = INT_MAX;

		return size;
	}

	static constexpr Padding ClampNegativePaddingToZero(Padding padding)
	{
		if(padding.GetAll() == 0)
		{
			padding.Left = (std::max)(0, padding.Left);
			padding.Top = (std::max)(0, padding.Top);
			padding.Right = (std::max)(0, padding.Right);
			padding.Bottom = (std::max)(0, padding.Bottom);
		}

		return padding;
	}

	static constexpr Drawing::Rectangle DeflateRect(Drawing::Rectangle rect, Padding padding)
	{
		rect.X += padding.Left;
		rect.Y += padding.Top;
		rect.Width -= padding.GetHorizontal();
		rect.Height -= padding.GetVertical();

		return rect;
	}

	static constexpr Size UnionSizes(Size a, Size b)
	{
		return Size((std::max)(a.Width, b.Width), (std::max)(a.Height, b.Height));
	}
};