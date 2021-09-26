#include "Byte.h"
#include "Boolean.h"
#include "Char.h"
#include "Int16.h"
#include "UInt16.h"
#include "Int32.h"
#include "Exceptions.h"

inline int Byte::GetHashCode() const
{
	return m_value;
}

int Byte::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Byte*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (unsigned char*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeByte");
}

int Byte::CompareTo(const Byte* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Byte::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Byte*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (unsigned char*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Byte::Equals(const Byte* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Byte::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Byte::ToChar(IFormatProvider* provider) const
{
	return Char();
}

Byte Byte::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Byte::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Byte::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Byte::ToInt32(IFormatProvider* provider) const
{
	return 0;
}