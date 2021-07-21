#include "Base.h"

Base::Base()
	:
	m_UUID(uuids::uuid_system_generator{}())
{

}

Base::~Base()
{

}

bool Base::Equals(Base b) const noexcept
{
	return m_UUID == b.m_UUID;
}

const std::string& Base::GetHashCode() const noexcept
{
	return uuids::to_string(m_UUID);
}

const Type Base::GetType() const noexcept
{
	return Type(typeid(*this));
}

const bool Base::ReferenceEquals(const Base& b) const noexcept
{
	return std::addressof(*this) == std::addressof(b);
}

const std::string Base::ToString() const noexcept
{
	// Default ToString method return it's type, unless the function is overriding in the derived class.
	return Type(typeid(*this)).ToString();
}