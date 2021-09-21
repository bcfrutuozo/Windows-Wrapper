#pragma once

#include "ValueType.h"
#include "IConvertible.h"

struct Single : public ValueType<Single, float>, public IConvertible
{
public:

	constexpr Single(float value);

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Single; }

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

	static Single MinValue() { return static_cast<float>(-3.40282346638528859e+38); }
	static Single Epsilon() { return static_cast<float>(1.4e-45); }
	static Single MaxValue() { return static_cast<float>(3.40282346638528859e+38); }
	static Single PositiveInfinity() { return static_cast<float>(1.0) / static_cast<float>(0.0); }
	static Single NegativeInfinity() { return static_cast<float>(-1.0) / static_cast<float>(0.0); }
	static Single NaN() { return static_cast<float>(0.0) / static_cast<float>(0.0); }
};