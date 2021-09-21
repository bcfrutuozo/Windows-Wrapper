#pragma once

#include "ValueType.h"
#include "IConvertible.h"

struct Char : public ValueType<Char, char>, public IConvertible
{
public:

	constexpr Char(char value);

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Char; }

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

	static bool IsLatin1(char ch) noexcept;
	static bool IsASCII(char ch) noexcept;
	static bool IsDigit(char ch) noexcept;
	static bool IsLetter(char ch) noexcept;

	static Char MaxValue() { return Char(0xFFFF); }
	static Char MinValue() { return Char(0); }
};