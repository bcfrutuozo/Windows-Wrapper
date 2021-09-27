#include "Int64.h"
#include "Int32.h"
#include "Boolean.h"
#include "SByte.h"
#include "Byte.h"
#include "Char.h"
#include "Int16.h"
#include "UInt16.h"
#include "UInt32.h"
#include "UInt64.h"
#include "Single.h"
#include "Double.h"
#include "Exceptions.h"

inline int Int64::GetHashCode() const
{
	return m_value;
}

int Int64::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Int64*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (long long*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeInt32");
}

int Int64::CompareTo(const Int64* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Int64::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Int64*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (long long*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Int64::Equals(const Int64* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Int64::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Int64::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte Int64::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte Int64::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Int64::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Int64::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Int64::ToInt32(IFormatProvider* provider) const
{
	return *this;
}

UInt32 Int64::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}


Int64 Int64::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 Int64::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single Int64::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double Int64::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
