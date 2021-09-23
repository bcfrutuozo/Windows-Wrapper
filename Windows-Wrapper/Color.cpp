#include "Color.h"
#include "Exceptions.h"

void Color::SetR(uint8_t value)
{
	*this = Color(value, GetG(), GetB(), GetA());
}

void Color::SetG(uint8_t value)
{
	*this = Color(GetR(), value, GetB(), GetA());
}

void Color::SetB(uint8_t value)
{
	*this = Color(GetR(), GetG(), value, GetA());
}

void Color::SetA(uint8_t value)
{
	*this = Color(GetR(), GetG(), GetB(), value);
}

inline bool Color::Equals(const Object* const c) const
{
	if (c == nullptr) return c;

	if (const auto clr = dynamic_cast<const Color*>(c))
	{
		return rgba == clr->rgba;
	}

	throw ArgumentException("Arg_MustBeColor");
}

inline bool Color::Equals(const Color* const c) const
{
	if (c == nullptr) return c;
	return rgba == c->rgba;
}

inline bool Color::operator==(const Color& c) const
{
	return rgba == c.rgba;
}