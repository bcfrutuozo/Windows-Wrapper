#include "SByte.h"
#include "Boolean.h"
#include "Char.h"
#include "Byte.h"
#include "Int16.h"
#include "UInt16.h"
#include "Int32.h"
#include "UInt32.h"
#include "Int64.h"
#include "UInt64.h"
#include "Single.h"
#include "Double.h"
#include "Exceptions.h"

inline int SByte::GetHashCode() const
{
	return m_value;
}

int SByte::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const SByte*>(obj))
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

	throw ArgumentException("Arg_MustBeSByte");
}

int SByte::CompareTo(const SByte* const value) const
{
	if (value == nullptr) return 1;

	if (m_value < value->Get()) return -1;
	if (m_value > value->Get()) return 1;

	return 0;
}

bool SByte::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const SByte*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (unsigned char*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool SByte::Equals(const SByte* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean SByte::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char SByte::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte SByte::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte SByte::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 SByte::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 SByte::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 SByte::ToInt32(IFormatProvider* provider) const
{
	return 0;
}

UInt32 SByte::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}

Int64 SByte::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 SByte::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single SByte::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double SByte::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
