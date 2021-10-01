#include "PointF.h"
#include "Point.h"
#include "Size.h"
#include "SizeF.h"
#include "Exceptions.h"

inline PointF PointF::Add(Point p, Size s) noexcept
{
	return PointF(static_cast<float>(p.X + s.Width), static_cast<float>(p.Y + s.Height));
}

inline PointF PointF::Add(Point p, SizeF s) noexcept
{
	return PointF(static_cast<float>(p.X) + s.Width, static_cast<float>(p.Y) + s.Height);
}

inline PointF PointF::Subtract(PointF p, Size s) noexcept
{
	return PointF(p.X - static_cast<float>(s.Width), p.Y - static_cast<float>(s.Height));
}

inline PointF PointF::Subtract(PointF p, SizeF s) noexcept
{
	return PointF(p.X - s.Width, p.Y - s.Height);
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