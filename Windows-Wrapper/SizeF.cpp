#include "SizeF.h"
#include "PointF.h"
#include "Size.h"
#include "Exceptions.h"

SizeF::SizeF(PointF p)
	:
	Width(p.X),
	Height(p.Y)
{
}

inline PointF SizeF::ToPointF() noexcept
{
	return PointF(Width, Height);
}

inline Size SizeF::ToSize() noexcept
{
	return Size(static_cast<int>(Width), static_cast<int>(Height));
}

bool SizeF::Equals(const Object* const s) const
{
	if (s == nullptr) return false;

	if (const auto sz = dynamic_cast<const SizeF*>(s))
	{
		return *this == *sz;
	}

	throw ArgumentException("Arg_MustBeSizeF");
}

bool SizeF::Equals(const SizeF* const s) const
{
	if (s == nullptr) return false;
	return *this == *s;
}