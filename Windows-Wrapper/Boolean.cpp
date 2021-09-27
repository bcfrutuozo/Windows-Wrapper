#include "Boolean.h"
#include "Char.h"
#include "SByte.h"
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

	if (const auto value = (bool*)(obj))
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

Char Boolean::ToChar(IFormatProvider* provider) const
{
	return Char();
}

SByte Boolean::ToSByte(IFormatProvider* provider) const
{
	return SByte();
}

Byte Boolean::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Boolean::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Boolean::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Boolean::ToInt32(IFormatProvider* provider) const
{
	// CONVERT BOOL TO INT32
	return 0;
}

UInt32 Boolean::ToUInt32(IFormatProvider* provider) const
{
	return UInt32();
}

Int64 Boolean::ToInt64(IFormatProvider* provider) const
{
	return Int64();
}

UInt64 Boolean::ToUInt64(IFormatProvider* provider) const
{
	return UInt64();
}

Single Boolean::ToSingle(IFormatProvider* provider) const
{
	return Single();
}

Double Boolean::ToDouble(IFormatProvider* provider) const
{
	return Double();
}
