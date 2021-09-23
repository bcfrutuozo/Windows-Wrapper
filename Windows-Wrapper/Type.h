#pragma once

#include "Object.h"
#include "IEquatable.h"

#include <string>
#include <typeinfo>

class Type : public Object, public IEquatable<Type>
{
	friend class Object;

private:

	const type_info& m_Type;

	Type(const type_info& t);

public:

	bool operator==(const Type& t) const;
	bool Equals(const Object* const b) const override;
	bool Equals(const Type* const t) const override;
	bool operator==(const type_info& t) const noexcept;
	const std::string ToString() const noexcept;
};