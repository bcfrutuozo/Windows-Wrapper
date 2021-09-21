#pragma once

#include "ValueType.h"
#include "IConvertible.h"

struct UInt64 : public ValueType<UInt64, uint64_t>, public IConvertible
{
public:

	constexpr UInt64(uint64_t value);

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::UInt64; }

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

	static UInt64 MaxValue() { return UInt64(0xffffffffffffffffL); }
	static UInt64 MinValue() { return UInt64(0L); }
};