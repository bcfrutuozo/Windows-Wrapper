#pragma once

#include "Common.h"

class Type;
class ListItem;

class Object
{
public:

	constexpr Object() = default;

	virtual bool operator==(const Object& b) const;
	virtual bool Equals(const Object* const b) const;
	bool static Equals(const Object* const lhs, const Object* const rhs);
	inline const int GetHashCode() const noexcept;
	inline const Type GetType() const noexcept;
	const bool ReferenceEquals(const Object& b) const noexcept;
	inline virtual const std::string ToString() const noexcept;
	virtual ListItem ToListItem() const noexcept;
};