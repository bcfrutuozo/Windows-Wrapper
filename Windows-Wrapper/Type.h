#pragma once

#include "IEquatable.h"

#include <string>
#include <typeinfo>

class Type : public IEquatable<Type>
{
private:

	const type_info& m_Type;

public:

	Type(const type_info& t)
		:
		m_Type(t)
	{}

	bool Equals(Type t) const noexcept
	{
		return m_Type == t.m_Type;
	}

	bool operator==(Type t) const noexcept
	{
		return m_Type == t.m_Type;
	}

	bool operator!=(Type t) const noexcept
	{
		return m_Type != t.m_Type;
	}

	const std::string ToString() const noexcept
	{
		return m_Type.name();
	}
};