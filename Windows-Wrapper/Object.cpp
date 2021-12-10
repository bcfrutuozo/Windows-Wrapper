#include "Object.h"
#include "ListItem.h"
#include "Exceptions.h"
#include "Type.h"
#include "Guid.h"

#include <string>

bool Object::Equals(const Object* const b) const
{
	if (b == nullptr) return false;
	return ReferenceEquals(this, b);
}

int Object::GetHashCode() const
{
	auto guid = uuid_system_generator{}();

	int ret = 0;
	for (const auto& i : guid)
	{
		ret ^= i;
	}

	return ret;
}

const Type Object::GetType() const noexcept
{
	return Type(typeid(*this));
}

std::string Object::ToString() const noexcept
{
	// Default ToString method return it's type, unless the function is overriden in the derived class.
	return GetType().ToString();
}

ListItem Object::ToListItem() const
{
	throw NotImplementedException("ToListItem is not implemented in default Object class.");
}
