#include "Int16.h"
#include "Boolean.h"
#include "SByte.h"
#include "Byte.h"
#include "Char.h"
#include "UInt16.h"
#include "Int32.h"
#include "UInt32.h"
#include "Int64.h"
#include "UInt64.h"
#include "Single.h"
#include "Double.h"
#include "Exceptions.h"

inline int Int16::GetHashCode() const
{
	return m_value;
}

int Int16::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Int16*>(obj))
	{
		int i = value->Get();

		if (m_value < i) return -1;
		if (m_value > i) return 1;

		return 0;
	}

	if (const auto value = (short*)(obj))
	{
		if (m_value < *value) return -1;
		if (m_value > *value) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeInt16");
}

int Int16::CompareTo(const Int16* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool Int16::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Int16*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (short*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Int16::Equals(const Int16* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Int16::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Int16::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte Int16::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte Int16::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Int16::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Int16::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Int16::ToInt32(IFormatProvider* provider) const
{
	return 0;
}

UInt32 Int16::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}

Int64 Int16::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 Int16::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single Int16::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double Int16::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
