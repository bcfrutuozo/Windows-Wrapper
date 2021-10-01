#include "Padding.h"
#include "Size.h"
#include "Exceptions.h"

bool Padding::Equals(const Object* const p) const
{
	if (p == nullptr) return false;

	if (const auto pad = dynamic_cast<const Padding*>(p))
	{
		return *this == *pad;
	}

	throw ArgumentException("Arg_MustBePadding");
}

bool Padding::Equals(const Padding* const s) const
{
	if (s == nullptr) return false;
	return *this == *s;
}