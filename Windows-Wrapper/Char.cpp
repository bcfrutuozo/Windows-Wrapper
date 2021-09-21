#pragma once

#include "Char.h"

Char::Char(char value)
	:
	ValueType(value)
{

}

Boolean Char::ToBoolean(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Char Char::ToChar(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

SByte Char::ToSByte(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Byte Char::ToByte(IFormatProvider* provider) const
{
	throw NotImplementedException();
}
Int16 Char::ToInt16(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

UInt16 Char::ToUInt16(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Int32 Char::ToInt32(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

UInt32 Char::ToUInt32(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Int64 Char::ToInt64(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

UInt64 Char::ToUInt64(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Single Char::ToSingle(IFormatProvider* provider)const
{
	throw NotImplementedException();
}

Double Char::ToDouble(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

Decimal Char::ToDecimal(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

DateTime Char::ToDateTime(IFormatProvider* provider) const
{
	throw NotImplementedException();
}

//String Char::ToString(IFormatProvider provider) const override
//{
//
//}

Object Char::ToType(Type conversionType, IFormatProvider* provider) const
{
	throw NotImplementedException();
}

bool Char::IsLatin1(char ch) noexcept 
{ 
	return ch <= '\x00ff'; 
}

bool Char::IsASCII(char ch) noexcept 
{ 
	return ch <= '\x007f'; 
}

bool Char::IsDigit(char ch) noexcept 
{ 
	return IsLatin1(ch) && ch >= '0' && ch <= '9' ? true : false; 
}

bool Char::IsLetter(char ch) noexcept
{
	if (IsLatin1(ch))
	{
		if (IsASCII(ch))
		{
			ch |= 0x20;
			return ch >= 'a' && ch <= 'z';
		}
	}
}