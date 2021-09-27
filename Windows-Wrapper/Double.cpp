#include "Double.h"
#include "Int32.h"
#include "Boolean.h"
#include "SByte.h"
#include "Byte.h"
#include "Char.h"
#include "Int16.h"
#include "UInt16.h"
#include "UInt32.h"
#include "Int64.h"
#include "UInt64.h"
#include "Single.h"
#include "Exceptions.h"

inline int Double::GetHashCode() const
{
	long double d = m_value;
	if (d == 0) {
		// Ensure that 0 and -0 have the same hash code
		return 0;
	}
	long value = *(long*)(&d);
	return ((int)value) ^ ((int)(value >> 32));
}

int Double::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Double*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (long double*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeInt32");
}

int Double::CompareTo(const Double* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Double::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Double*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (long double*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Double::Equals(const Double* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Double::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Double::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte Double::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte Double::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Double::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Double::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Double::ToInt32(IFormatProvider* provider) const
{
	return *this;
}

UInt32 Double::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}

Int64 Double::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 Double::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single Double::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double Double::ToDouble(IFormatProvider* provider) const
{
	return 0;
}
