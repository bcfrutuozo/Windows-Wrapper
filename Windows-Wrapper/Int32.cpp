#include "Int32.h"
#include "Int16.h"
#include "Boolean.h"
#include "Exceptions.h"

// Int32::Int32(const Char& value) noexcept
// :
//	Primitive(value.Get())
//{
//
//}
// Int32::Int32(const SByte& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
// Int32::Int32(const Byte& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
// Int32::Int32(const Int16& value) noexcept
//:
//Primitive(value.Get())
//{
//
//}
//Int32::Int32(const Int64& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
//Int32::Int32(const Single& value) noexcept
// :
//Primitive(value.Get())
//{
//
//}
//Int32::Int32(const Double& value) noexcept
//:
//Primitive(value.Get())
//{
//
//}

//Int32 Int32::operator+(Boolean const& other) const noexcept
//{
//	return Boolean(m_value + other.Get());
//}

inline int Int32::GetHashCode() const
{
	return m_value;
}

int Int32::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Int32*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (int32_t*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeInt32");
}

int Int32::CompareTo(const Int32* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Int32::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Int32*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (int32_t*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Int32::Equals(const Int32* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Int32::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Int16 Int32::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

Int32 Int32::ToInt32(IFormatProvider* provider) const
{
	return *this;
}