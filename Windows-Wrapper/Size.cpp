#include "Size.h"
#include "Point.h"
#include "SizeF.h"
#include "Exceptions.h"

Size::Size(Point p)
	:
	Width(p.X),
	Height(p.Y)
{
}

inline Size Size::CeilingF(SizeF s) noexcept
{
	return Size(static_cast<int>(ceil(s.Width)), static_cast<int>(ceil(s.Height)));
}

inline Size Size::RoundF(SizeF s) noexcept
{
	return Size(static_cast<int>(round(s.Width)), static_cast<int>(round(s.Height)));
}

inline Size Size::Truncate(SizeF s) noexcept
{
	return Size(static_cast<int>(trunc(s.Width)), static_cast<int>(trunc(s.Height)));
}

int Size::GetHashCode() const noexcept
{
	return Width ^ Height;
}

bool Size::Equals(const Object* const s) const
{
	if (s == nullptr) return false;

	if (const auto sz = dynamic_cast<const Size*>(s))
	{
		return *this == *sz;
	}

	throw ArgumentException("Arg_MustBeSize");
}

bool Size::Equals(const Size* const s) const
{
	if (s == nullptr) return false;
	return *this == *s;
}