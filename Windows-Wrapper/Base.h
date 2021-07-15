#pragma once

#include "Type.h"
#include "uuid.h"

#include <memory>
#include <string>

class Base : public IEquatable<Base>
{
	//friend class Control;

private:

	uuids::uuid m_UUID;

protected:

	Base()
		:
		m_UUID(uuids::uuid_system_generator{}())
	{}

public:

	bool Equals(Base b) const noexcept
	{
		return m_UUID == b.m_UUID;
	}

	const std::string& GetHashCode() const noexcept
	{
		return uuids::to_string(m_UUID);
	}

	const Type GetType() const noexcept
	{
		return Type(typeid(*this));
	}

	virtual const std::string ToString() const noexcept
	{
		return Type(typeid(*this)).ToString();
	}
};