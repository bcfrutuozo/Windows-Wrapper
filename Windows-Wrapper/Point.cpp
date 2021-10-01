#include "Point.h"
#include "PointF.h"
#include "Size.h"
#include "Exceptions.h"

Point::Point(Size size) 
	: 
	X(size.Width), 
	Y(size.Height)
{

}

inline Point Point::Add(Point p, Size s) noexcept
{
	return Point(p.X + s.Width, p.Y + s.Height);
}

inline Point Point::CeilingF(PointF p) noexcept
{
	return Point(static_cast<int>(ceil(p.X)), static_cast<int>(ceil(p.Y)));
}

inline Point Point::Offset(int x, int y) noexcept
{
	return Point(X += x, Y += y);
}

inline Point Point::Offset(Point p) noexcept
{
	return Point(X += p.X, Y += p.Y);
}

inline Point Point::RoundF(PointF p) noexcept
{
	return Point(static_cast<int>(round(p.X)), static_cast<int>(round(p.Y)));
}

inline Point Point::Subtract(Point p, Size s) noexcept
{
	return Point(p.X - s.Width, p.Y - s.Height);
}

inline Point Point::Truncate(PointF p) noexcept
{
	return Point(static_cast<int>(trunc(p.X)), static_cast<int>(trunc(p.Y)));
}

int Point::GetHashCode() const noexcept
{
	return X ^ Y;
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