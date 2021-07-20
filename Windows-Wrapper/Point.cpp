#include "Point.h"
#include "Size.h"

#define LOWORD(l)	((l) & 0xffff)
#define HIWORD(l)   (((l) >> 16) & 0xffff)

// Point
Point::Point(int word)
	:
	X(LOWORD(word)),
	Y(HIWORD(word))
{
}

Point::Point(int x, int y)
	:
	X(x),
	Y(y)
{

}

Point::Point(Size size)
	:
	X(size.Width),
	Y(size.Height)
{

}

bool Point::Equals(Point p) const noexcept
{
	return (X == p.X && Y == p.Y);
}

const bool Point::IsEmpty() const noexcept
{
	if (X == 0 && Y == 0)
	{
		return true;
	}

	return false;
}

Point Point::Add(Point p, Size s) noexcept
{
	return Point(p.X + s.Width, p.Y + s.Height);
}

Point Point::CeilingF(PointF p) noexcept
{
	return Point(ceil(p.X), ceil(p.Y));
}

Point Point::Offset(int x, int y) noexcept
{
	return Point(X += x, Y += y);
}

Point Point::Offset(Point p) noexcept
{
	return Point(X += p.X, Y += p.Y);
}

Point Point::RoundF(PointF p) noexcept
{
	return Point(round(p.X), round(p.Y));
}

Point Point::Subtract(Point p, Size s) noexcept
{
	return Point(p.X - s.Width, p.Y - s.Height);
}

Point Point::Truncate(PointF p) noexcept
{
	return Point(trunc(p.X), trunc(p.Y));
}

Point Point::operator+(Size s) noexcept
{
	return Point(X + s.Width, Y + s.Height);
}

bool Point::operator==(Point p) noexcept
{
	return (X == p.X && Y == p.Y);
}

bool Point::operator!=(Point p) noexcept
{
	return (X != p.X || Y != p.Y);
}

Point Point::operator-(Size s) noexcept
{
	return Point(X - s.Width, Y - s.Height);
}

// PointF
PointF::PointF(float x, float y)
	:
	X(x),
	Y(y)
{

}

bool PointF::Equals(PointF p) const noexcept
{
	return (X == p.X && Y == p.Y);
}

const bool PointF::IsEmpty() const noexcept
{
	if (X == 0 && Y == 0)
	{
		return true;
	}

	return false;
}

PointF PointF::Add(Point p, Size s) noexcept
{
	return PointF(p.X + s.Width, p.Y + s.Height);
}

PointF PointF::Add(Point p, SizeF s) noexcept
{
	return PointF(p.X + s.Width, p.Y + s.Height);
}


PointF PointF::Subtract(PointF p, Size s) noexcept
{
	return PointF(p.X - s.Width, p.Y - s.Height);
}

PointF PointF::Subtract(PointF p, SizeF s) noexcept
{
	return PointF(p.X - s.Width, p.Y - s.Height);
}

PointF PointF::operator+(Size s) noexcept
{
	return PointF(X + s.Width, Y + s.Height);
}

PointF PointF::operator+(SizeF s) noexcept
{
	return PointF(X + s.Width, Y + s.Height);
}

bool PointF::operator==(PointF p) noexcept
{
	return (X == p.X && Y == p.Y);
}

bool PointF::operator!=(PointF p) noexcept
{
	return (X != p.X || Y != p.Y);
}

PointF PointF::operator-(Size s) noexcept
{
	return PointF(X - s.Width, Y - s.Height);
}

PointF PointF::operator-(SizeF s) noexcept
{
	return PointF(X - s.Width, Y - s.Height);
}