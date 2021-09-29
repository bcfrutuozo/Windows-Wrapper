#include "Point.h"
#include "Size.h"
#include "Exceptions.h"

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

bool Point::operator==(const Point& p) const 
{
	return (X == p.X && Y == p.Y);
}

bool Point::Equals(const Object* const p) const
{
	if (p == nullptr) return false;

	if (const auto pt = dynamic_cast<const Point*>(p))
	{
		return *this == *pt;
	}

	throw ArgumentException("Arg_MustBePoint");
}

bool Point::Equals(const Point* const p) const 
{
	if (p == nullptr) return false;
	return *this == *p;
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
	return Point(static_cast<int>(ceil(p.X)), static_cast<int>(ceil(p.Y)));
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
	return Point(static_cast<int>(round(p.X)), static_cast<int>(round(p.Y)));
}

Point Point::Subtract(Point p, Size s) noexcept
{
	return Point(p.X - s.Width, p.Y - s.Height);
}

Point Point::Truncate(PointF p) noexcept
{
	return Point(static_cast<int>(trunc(p.X)), static_cast<int>(trunc(p.Y)));
}

Point Point::operator+(Size s) noexcept
{
	return Point(X + s.Width, Y + s.Height);
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

bool PointF::operator==(const PointF& p) const
{
	return (X == p.X && Y == p.Y);
}

bool PointF::Equals(const Object* const p) const
{
	if (p == nullptr) return false;

	if (const auto pt = dynamic_cast<const PointF*>(p))
	{
		return *this == *pt;
	}

	throw ArgumentException("Arg_MustBePointF");
}

bool PointF::Equals(const PointF* const p) const
{
	if (p == nullptr) return false;
	return *this == *p;
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
	return PointF(static_cast<float>(p.X + s.Width), static_cast<float>(p.Y + s.Height));
}

PointF PointF::Add(Point p, SizeF s) noexcept
{
	return PointF(static_cast<float>(p.X) + s.Width, static_cast<float>(p.Y) + s.Height);
}

PointF PointF::Subtract(PointF p, Size s) noexcept
{
	return PointF(p.X - static_cast<float>(s.Width), p.Y - static_cast<float>(s.Height));
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

PointF PointF::operator-(Size s) noexcept
{
	return PointF(X - s.Width, Y - s.Height);
}

PointF PointF::operator-(SizeF s) noexcept
{
	return PointF(X - s.Width, Y - s.Height);
}