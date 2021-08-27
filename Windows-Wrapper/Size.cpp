#include "Size.h"
#include "Point.h"

#include <cassert>

// Size
Size::Size(int width, int height)
	:
	Width(width),
	Height(height)
{

}

Size::Size(Point p)
	:
	Width(p.X),
	Height(p.Y)
{

}

bool Size::operator==(const Size& s) const noexcept
{
	return (Width == s.Width && Height == s.Height);
}

bool Size::Equals(const Size& s) const noexcept
{
	if (Width == s.Width && Height == s.Height)
	{
		return true;
	}

	return false;
}

const bool Size::IsEmpty() const noexcept
{
	if (Width == 0 && Height == 0)
	{
		return true;
	}

	return false;
}

Size Size::Add(Size lhs, Size rhs) noexcept
{
	return Size(lhs.Width + rhs.Width, lhs.Height + rhs.Height);
}

Size Size::CeilingF(SizeF s) noexcept
{
	return Size(ceil(s.Width), ceil(s.Height));
}

Size Size::RoundF(SizeF s) noexcept
{
	return Size(round(s.Width), round(s.Height));
}

Size Size::Subtract(Size lhs, Size rhs) noexcept
{
	return Size(lhs.Width - rhs.Width, lhs.Height - rhs.Height);
}

Size Size::Truncate(SizeF s) noexcept
{
	return Size(trunc(s.Width), trunc(s.Height));
}

Size Size::operator+(Size s) noexcept
{
	return Size(Width + s.Width, Height + s.Height);
}

Size Size::operator/(int i)
{
	assert(i != 0);
	return Size(Width / i, Height / i);
}

Size Size::operator/(float f)
{
	assert(f != 0);
	return Size(Width / f, Height / f);
}

Size Size::operator*(int i) noexcept
{
	return Size(Width * i, Height * i);
}

SizeF Size::operator*(float f) noexcept
{
	return SizeF(Width * f, Height * f);
}

Size Size::operator*(Size s) noexcept
{
	return Size(Width * s.Width, Height * s.Height);
}

SizeF Size::operator*(SizeF s) noexcept
{
	return SizeF(Width * s.Width, Height * s.Height);
}

Size Size::operator-(Size s) noexcept
{
	return Size(Width - s.Width, Height - s.Height);
}

// SizeF
SizeF::SizeF(float width, float height)
	:
	Width(width),
	Height(height)
{

}

SizeF::SizeF(PointF p)
	:
	Width(p.X),
	Height(p.Y)
{

}

bool SizeF::operator==(const SizeF& s) const noexcept
{
	return (Width == s.Width && Height == s.Height);
}

bool SizeF::Equals(const SizeF& s) const noexcept
{
	if (Width == s.Width && Height == s.Height)
	{
		return true;
	}

	return false;
}

const bool SizeF::IsEmpty() const noexcept
{
	if (Width == 0 && Height == 0)
	{
		return true;
	}

	return false;
}

SizeF SizeF::Add(SizeF lhs, SizeF rhs) noexcept
{
	return SizeF(lhs.Width + rhs.Width, lhs.Height + rhs.Height);
}

SizeF SizeF::Subtract(SizeF lhs, SizeF rhs) noexcept
{
	return SizeF(lhs.Width - rhs.Width, lhs.Height - rhs.Height);
}

PointF SizeF::ToPointF() noexcept
{
	return PointF(Width, Height);
}

Size SizeF::ToSize() noexcept
{
	return Size(Width, Height);
}

SizeF SizeF::operator+(SizeF s) noexcept
{
	return SizeF(Width + s.Width, Height + s.Height);
}

SizeF SizeF::operator/(float f) noexcept
{
	assert(f != 0);
	return SizeF(Width * f, Height * f);
}

SizeF SizeF::operator*(float f) noexcept
{
	return SizeF(Width * f, Height * f);
}

SizeF SizeF::operator*(SizeF s) noexcept
{
	return SizeF(Width * s.Width, Height * s.Height);
}

SizeF SizeF::operator-(SizeF s) noexcept
{
	return SizeF(Width - s.Width, Height - s.Height);
}