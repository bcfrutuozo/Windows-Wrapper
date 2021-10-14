#include "RectangleF.h"
#include "Exceptions.h"

#include <sstream>

namespace Drawing
{
	int RectangleF::GetHashCode() const
	{
		return ((int)((uint32_t)Left ^
			(((uint32_t)Top << 13) | ((uint32_t)Top >> 19)) ^
			(((uint32_t)Right << 26) | ((uint32_t)Right >> 6)) ^
			(((uint32_t)Bottom << 7) | ((uint32_t)Bottom >> 25))));
	}

	bool RectangleF::Equals(const Object* const obj) const
	{
		if (obj == nullptr) return false;

		if (const auto r = dynamic_cast<const RectangleF*>(obj))
		{
			return (r->Left == Left) && (r->Top == Top) && (r->Right == Right) && (r->Bottom == Bottom);
		}

		throw ArgumentException("Arg_MustBeRectangle");
	}

	bool RectangleF::Equals(const RectangleF* const b) const
	{
		if (b == nullptr) return false;
		return (b->Left == Left) && (b->Top == Top) && (b->Right == Right) && (b->Bottom == Bottom);
	}

	std::string RectangleF::ToString() const noexcept
	{
		std::ostringstream oss;
		oss << "{X=" << Left << ",Y=" << Top << ",Width=" << Right << ",Height=" << Bottom << "}";
		return oss.str();
	}
}