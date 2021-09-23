#include "Type.h"
#include "Exceptions.h"

bool Type::operator==(const Type& t) const noexcept
{
	return m_Type == t.m_Type;
}

bool Type::operator==(const type_info& t) const noexcept
{
	return m_Type == t;
}

bool Type::Equals(const Object* const b) const
{
	if (b == nullptr) return false;

	if (const auto t = dynamic_cast<const Type*>(b))
	{
		return m_Type == t->m_Type;
	}

	throw ArgumentException("Arg_MustBeType");
}

bool Type::Equals(const Type* const t) const
{
	if (t == nullptr) return false;
	return m_Type == t->m_Type;
}

const std::string Type::ToString() const noexcept
{
	return m_Type.name();
}