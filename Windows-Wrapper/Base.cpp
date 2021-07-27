#include "Base.h"

Base::Base()
	:
	m_UUID(uuids::uuid_system_generator{}())
{

}

Base::~Base()
{

}

inline bool Base::operator==(const Base& b) const noexcept
{
	return *this == b;
}

inline bool Base::Equals(const Base& lhs, const Base& rhs) noexcept
{
	return lhs.Equals(rhs);
}

inline bool Base::Equals(const Base& b) const noexcept
{
	return *this == b;
}

inline const std::string& Base::GetHashCode() const noexcept
{
	return uuids::to_string(m_UUID);
}

inline const Type Base::GetType() const noexcept
{
	return Type(typeid(*this));
}

inline const bool Base::ReferenceEquals(const Base& b) const noexcept
{
	return std::addressof(*this) == std::addressof(b);
}

inline const std::string Base::ToString() const noexcept
{
	// Default ToString method return it's type, unless the function is override in the derived class.
	return GetType().ToString();
}