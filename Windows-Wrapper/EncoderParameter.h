#pragma once

#include "IDisposable.h"
#include "IntPtr.h"
#include "Encoder.h"

class EncoderParameter final : public IDisposable
{
private:

	enum class EncoderParameterValueType
	{
		ValueTypeByte = 1,
		ValueTypeAscii = 2,
		ValueTypeShort = 3,
		ValueTypeLong = 4,
		ValueTypeRational = 5,
		ValueTypeLongRange = 6,
		ValueTypeUndefined = 7,
		ValueTypeRationalRange = 8
	};

	Guid parameterGuid;
	int numberOfValues;
	EncoderParameterValueType parameterValueType;
	IntPtr parameterValue;

	void Dispose(bool disposing);

	static constexpr IntPtr Add(IntPtr a, int b) { return IntPtr(a.ToInt64() + (long)b); }
	static constexpr IntPtr Add(int a, IntPtr b) { return IntPtr((long)a + b.ToInt64()); }

public:

	EncoderParameter(Encoder encoder, byte value);
	~EncoderParameter();

	void Dispose() override;

	void SetEncoder(Encoder e);

	constexpr Encoder GetEncoder() { return Encoder(parameterGuid); }
	constexpr EncoderParameterValueType GetValueType() { return parameterValueType; }
	constexpr int NumberOfValues() { return numberOfValues; }
};