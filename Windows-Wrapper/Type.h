#pragma once

#include "IEquatable.h"

class Type : public IEquatable<Type>
{
	friend class Object;

private:

	const type_info& m_Type;

	constexpr Type(const type_info& t) noexcept : m_Type(t) { }

public:

	bool operator==(const Type& t) const noexcept;
	bool operator==(const type_info& t) const noexcept;

	bool Equals(const Object* const b) const override;
	bool Equals(const Type* const t) const override;
	const std::string ToString() const noexcept;
};