#include "Object.h"
#include "ListItem.h"
#include "Exceptions.h"
#include "Type.h"
#include "Guid.h"

inline bool Object::operator==(const Object& b) const
{
	return *this == b;
}

inline bool Object::Equals(const Object* const b) const
{
	if (b == nullptr) return false;
	return *this == *b;
}

inline bool Object::Equals(const Object* const lhs, const Object* const rhs)
{
	if (lhs == nullptr && rhs == nullptr) return true;
	if (lhs == nullptr || rhs == nullptr) return false;

	return lhs->Equals(rhs);
}

inline int Object::GetHashCode() const noexcept
{
	auto guid = uuid_system_generator{}();

	int ret = 0;
	for (const auto& i : guid)
	{
		ret ^= i;
	}

	return ret;
}

inline const Type Object::GetType() const noexcept
{
	return Type(typeid(*this));
}

const bool Object::ReferenceEquals(const Object& b) const noexcept
{
	return std::addressof(*this) == std::addressof(b);
}

inline const std::string Object::ToString() const noexcept
{
	// Default ToString method return it's type, unless the function is overriden in the derived class.
	return GetType().ToString();
}

ListItem Object::ToListItem() const noexcept
{
	throw NotImplementedException("ToListItem is not implemented in default Object class.");
}
