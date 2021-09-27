#include "UInt64.h"
#include "Boolean.h"
#include "SByte.h"
#include "Byte.h"
#include "Char.h"
#include "Int16.h"
#include "UInt16.h"
#include "Int32.h"
#include "UInt32.h"
#include "Int64.h"
#include "Single.h"
#include "Double.h"
#include "Exceptions.h"

inline int UInt64::GetHashCode() const
{
	return m_value;
}

int UInt64::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const UInt64*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (unsigned long long*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeUInt32");
}

int UInt64::CompareTo(const UInt64* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool UInt64::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const UInt64*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (unsigned long long*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool UInt64::Equals(const UInt64* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean UInt64::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char UInt64::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte UInt64::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte UInt64::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 UInt64::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 UInt64::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 UInt64::ToInt32(IFormatProvider* provider) const
{
	return 0;
}

UInt32 UInt64::ToUInt32(IFormatProvider* provider) const
{
	return 0;
}

Int64 UInt64::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 UInt64::ToUInt64(IFormatProvider* provider) const
{
	return Int64();
}

Single UInt64::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double UInt64::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
