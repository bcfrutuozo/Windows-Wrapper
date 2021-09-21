#pragma once

#include "IEquatable.h"

#include <string>
#include <typeinfo>

class Type : public IEquatable<Type>
{
	friend class Object;

private:

	const type_info& m_Type;

	Type(const type_info& t);

public:

	bool operator==(const Type& t) const noexcept override;
	bool Equals(const Type& t) const noexcept override;
	bool operator==(const type_info& t) const noexcept;
	const std::string ToString() const noexcept;
};