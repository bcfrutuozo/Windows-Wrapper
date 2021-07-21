#pragma once

#include "IEquatable.h"

#include <string>
#include <typeinfo>

class Type : public IEquatable<Type>
{
	friend class Base;

private:

	const type_info& m_Type;

	Type(const type_info& t);

public:

	bool Equals(Type t) const noexcept;
	bool operator==(Type t) const noexcept;
	bool operator==(const type_info& t) const noexcept;
	virtual const std::string ToString() const noexcept;
};