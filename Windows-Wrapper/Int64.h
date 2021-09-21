#pragma once

#include "ValueType.h"
#include "IConvertible.h"

struct Int64 : public ValueType<Int64, int64_t>, public IConvertible
{
public:

	constexpr Int64(int64_t value);

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Int64; }

	Boolean ToBoolean(IFormatProvider* provider) const override;
	Char ToChar(IFormatProvider* provider) const override;
	SByte ToSByte(IFormatProvider* provider) const override;
	Byte ToByte(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	UInt16 ToUInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;
	UInt32 ToUInt32(IFormatProvider* provider) const override;
	Int64 ToInt64(IFormatProvider* provider) const override;
	UInt64 ToUInt64(IFormatProvider* provider) const override;
	Single ToSingle(IFormatProvider* provider)const override;
	Double ToDouble(IFormatProvider* provider) const override;
	Decimal ToDecimal(IFormatProvider* provider) const override;
	DateTime ToDateTime(IFormatProvider* provider) const override;
	//String ToString(IFormatProvider provider) const override;
	Object ToType(Type conversionType, IFormatProvider* provider) const override;

	static Int64 MaxValue() { return Int64(0x7fffffffffffffffL); }
	static Int64 MinValue() { return Int64(0x8000000000000000L); }
};