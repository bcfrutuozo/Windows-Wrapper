#pragma once

#include "IEquatable.h"

#include <string>
#include <typeinfo>

class Type : public IEquatable<Type>
{
private:

	const type_info& m_Type;

public:

	Type(const type_info& t);

	bool Equals(Type t) const noexcept;
	bool operator==(Type t) const noexcept;
	bool operator==(const type_info& t) const noexcept;
	bool operator!=(Type t) const noexcept;
	bool operator!=(const type_info& t) const noexcept;
	const std::string ToString() const noexcept;
};