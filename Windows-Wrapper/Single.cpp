#include "Single.h"
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
#include "Double.h"
#include "Exceptions.h"

inline int Single::GetHashCode() const
{
	float f = m_value;

	if (m_value == 0) {
		// Ensure that 0 and -0 have the same hash code
		return 0;
	}

	int v = *(int*)(&f);
	return v;
}

int Single::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Single*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (float*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeInt32");
}

int Single::CompareTo(const Single* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Single::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Single*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (float*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Single::Equals(const Single* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Single::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Single::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte Single::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte Single::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Single::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Single::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Single::ToInt32(IFormatProvider* provider) const
{
	return *this;
}

UInt32 Single::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}

Int64 Single::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 Single::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single Single::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double Single::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
