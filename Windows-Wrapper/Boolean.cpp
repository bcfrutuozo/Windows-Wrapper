#include "Boolean.h"
#include "Int32.h"
#include "Int16.h"
#include "Exceptions.h"

// Boolean::Boolean(const Char& value) noexcept
// :
//	Primitive(value.Get())
//{
//
//}
// Boolean::Boolean(const SByte& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
// Boolean::Boolean(const Byte& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
// Boolean::Boolean(const Int16& value) noexcept
//:
//Primitive(value.Get())
//{
//
//}
//Boolean::Boolean(const Int32& value) noexcept
//	:
//	Primitive(value.Get())
//{
//
//}
//Boolean::Boolean(const Int64& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
//Boolean::Boolean(const Single& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
//Boolean::Boolean(const Double& value) noexcept
//:
//Primitive(value.Get())
//{
//
//}

//Boolean Boolean::operator+(Int32 const& other) const noexcept
//{
//	return Boolean(m_value + other.Get());
//}

inline int Boolean::GetHashCode() const
{
	return m_value ? 1 : 0;
}

int Boolean::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Boolean*>(obj))
	{
		int i = value->Get();

		if (m_value == i) return 0;
		else if (m_value == false) return - 1;
		else return 1;
	}

	if (const auto value = (bool*)(obj))
	{
		if (m_value == *value) return 0;
		else if (m_value == false) return -1;
		else return 1;
	}

	throw ArgumentException("Arg_MustBeBoolean");
}

int Boolean::CompareTo(const Boolean* const value) const
{
	if (value == nullptr) return 1;

	if (m_value == value->Get()) return 0;
	else if (m_value == false) return -1;
	else return 1;
}

bool Boolean::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Boolean*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (int32_t*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Boolean::Equals(const Boolean* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Boolean::ToBoolean(IFormatProvider* provider) const
{
	return Boolean(m_value);
}

Int16 Boolean::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

Int32 Boolean::ToInt32(IFormatProvider* provider) const
{
	// CONVERT BOOL TO INT32
	return 0;
}