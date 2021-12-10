#pragma once

#include "Common.h"

#include <string>
#include <memory>

class Type;
class ListItem;

class Object
{
public:

	constexpr Object() 
	{ }

	constexpr static bool ReferenceEquals(const Object* a, const Object* b) noexcept { return std::addressof(*a) == std::addressof(*b); }

	constexpr static bool Equals(const Object* const lhs, const Object* const rhs)
	{
		if (lhs == nullptr && rhs == nullptr) return true;
		if (lhs == nullptr || rhs == nullptr) return false;

		return lhs->Equals(rhs);
	}

	virtual bool Equals(const Object* const b) const;
	virtual int GetHashCode() const;
	const Type GetType() const noexcept;
	virtual std::string ToString() const noexcept;
	virtual ListItem ToListItem() const;
};