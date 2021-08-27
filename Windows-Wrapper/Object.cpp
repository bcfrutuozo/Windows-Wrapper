#include "Object.h"

Object::Object() noexcept
	:
	m_UUID(uuids::uuid_system_generator{}())
{

}

inline bool Object::operator==(const Object& b) const noexcept
{
	return *this == b;
}

inline bool Object::Equals(const Object& b) const noexcept
{
	return *this == b;
}

inline bool Object::Equals(const Object& lhs, const Object& rhs) noexcept
{
	return lhs.Equals(rhs);
}

inline const std::string& Object::GetHashCode() const noexcept
{
	return uuids::to_string(m_UUID);
}

inline const Type Object::GetType() const noexcept
{
	return Type(typeid(*this));
}

inline const bool Object::ReferenceEquals(const Object& b) const noexcept
{
	return std::addressof(*this) == std::addressof(b);
}

inline const std::string Object::ToString() const noexcept
{
	// Default ToString method return it's type, unless the function is override in the derived class.
	return GetType().ToString();
}