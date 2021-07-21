#include "Type.h"

Type::Type(const type_info& t)
	:
	m_Type(t)
{}

bool Type::Equals(Type t) const noexcept
{
	return m_Type == t.m_Type;
}

bool Type::operator==(Type t) const noexcept
{
	return m_Type == t.m_Type;
}

bool Type::operator==(const type_info& t) const noexcept
{
	return m_Type == t;
}

const std::string Type::ToString() const noexcept
{
	return m_Type.name();
}