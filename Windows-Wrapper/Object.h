#pragma once

#include "Common.h"

class Type;
class ListItem;

class Object
{
public:

	constexpr Object() = default;

	constexpr bool operator==(const Object& b) const noexcept { return *this == b; }
	constexpr bool ReferenceEquals(const Object& b) const noexcept { return std::addressof(*this) == std::addressof(b); }

	bool constexpr static Equals(const Object* const lhs, const Object* const rhs)
	{
		if (lhs == nullptr && rhs == nullptr) return true;
		if (lhs == nullptr || rhs == nullptr) return false;

		return lhs->Equals(rhs);
	}

	virtual bool Equals(const Object* const b) const;
	inline virtual int GetHashCode() const noexcept;
	inline const Type GetType() const noexcept;
	inline virtual const std::string ToString() const noexcept;
	virtual ListItem ToListItem() const noexcept;
};