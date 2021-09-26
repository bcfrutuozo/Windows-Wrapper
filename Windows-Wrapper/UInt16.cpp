#include "UInt16.h"
#include "Boolean.h"
#include "Byte.h"
#include "Char.h"
#include "Int16.h"
#include "Int32.h"
#include "Exceptions.h"

inline int UInt16::GetHashCode() const
{
	return m_value;
}

int UInt16::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const UInt16*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (unsigned short*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeUInt16");
}

int UInt16::CompareTo(const UInt16* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool UInt16::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const UInt16*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (unsigned short*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool UInt16::Equals(const UInt16* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean UInt16::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char UInt16::ToChar(IFormatProvider* provider) const
{
	return Char();
}

Byte UInt16::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 UInt16::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 UInt16::ToUInt16(IFormatProvider* provider) const
{
	return 0;
}

Int32 UInt16::ToInt32(IFormatProvider* provider) const
{
	return 0;
}