#include "Rectangle.h"
#include "Exceptions.h"

namespace Drawing
{
	int Rectangle::GetHashCode() const
	{
		return ((int)((uint32_t)X ^
			(((uint32_t)Y << 13) | ((uint32_t)Y >> 19)) ^
			(((uint32_t)Width << 26) | ((uint32_t)Width >> 6)) ^
			(((uint32_t)Height << 7) | ((uint32_t)Height >> 25))));
	}

	bool Rectangle::Equals(const Object* const obj) const
	{
		if (obj == nullptr) return false;

		if (const auto r = dynamic_cast<const Rectangle*>(obj))
		{
			return (r->X == X) && (r->Y == Y) && (r->Width == Width) && (r->Height == Height);
		}

		throw ArgumentException("Arg_MustBeRectangle");
	}

	bool Rectangle::Equals(const Rectangle* const b) const
	{
		if (b == nullptr) return false;
		return (b->X == X) && (b->Y == Y) && (b->Width == Width) && (b->Height == Height);
	}
	const std::string Rectangle::ToString() const noexcept
	{
		std::ostringstream oss;
		oss << "{X=" << X << ",Y=" << Y << ",Width=" << Width << ",Height=" << Height << "}";
		return oss.str();
	}
}