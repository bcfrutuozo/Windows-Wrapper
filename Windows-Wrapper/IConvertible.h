#pragma once

#include "Object.h"
#include "IFormatProvider.h"
#include "TypeCode.h"

class Int32;

class IConvertible : public Object
{
public:
//
	inline virtual constexpr TypeCode GetTypeCode() const noexcept = 0;
//
//	virtual Boolean ToBoolean(IFormatProvider* provider) const = 0;
//	virtual Char ToChar(IFormatProvider* provider) const = 0;
//	virtual SByte ToSByte(IFormatProvider* provider) const = 0;
//	virtual Byte ToByte(IFormatProvider* provider) const = 0;
//	virtual Int16 ToInt16(IFormatProvider* provider) const = 0;
//	virtual UInt16 ToUInt16(IFormatProvider* provider) const = 0;
	virtual Int32 ToInt32(IFormatProvider* provider) const = 0;
//	virtual UInt32 ToUInt32(IFormatProvider* provider) const = 0;
//	virtual Int64 ToInt64(IFormatProvider* provider) const = 0;
//	virtual UInt64 ToUInt64(IFormatProvider* provider) const = 0;
//	virtual Single ToSingle(IFormatProvider* provider) const = 0;
//	virtual Double ToDouble(IFormatProvider* provider) const = 0;
//	virtual Decimal ToDecimal(IFormatProvider* provider) const = 0;
//	virtual DateTime ToDateTime(IFormatProvider* provider) const = 0;
//	//virtual String ToString(IFormatProvider provider) const = 0;
//	virtual Object ToType(Type conversionType, IFormatProvider* provider) const = 0;
};