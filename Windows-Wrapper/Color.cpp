#include "Color.h"
#include "Exceptions.h"

void Color::SetR(uint8_t value) noexcept
{
	*this = Color(value, GetG(), GetB(), GetA());
}

void Color::SetG(uint8_t value) noexcept
{
	*this = Color(GetR(), value, GetB(), GetA());
}

void Color::SetB(uint8_t value) noexcept
{
	*this = Color(GetR(), GetG(), value, GetA());
}

void Color::SetA(uint8_t value) noexcept
{
	*this = Color(GetR(), GetG(), GetB(), value);
}

int Color::GetHashCode() const
{
	int tmp;
	std::memcpy(&tmp, &rgba, sizeof(tmp));
	return tmp;
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

const std::string Color::ToString() const noexcept
{
	std::ostringstream oss;
	oss << "{{ARGB=(" << ((rgba >> 24) & 0xFF) << ", " << ((rgba >> 0) & 0xFF) << ", " << ((rgba >> 8) & 0xFF) << ", " << ((rgba >> 16) & 0xFF) << ")}}";
	return std::string();
}
