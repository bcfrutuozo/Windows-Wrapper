#include "EncoderParameter.h"
#include "Exceptions.h"

void EncoderParameter::Dispose(bool disposing)
{
	if (parameterValue != IntPtr::Zero())
	{
		GlobalFree(parameterValue.ToPointer());
		parameterValue = IntPtr::Zero();
	}
}

EncoderParameter::EncoderParameter(Encoder encoder, byte value)
{
	parameterGuid = encoder.GetGuid();

	parameterValueType = EncoderParameterValueType::ValueTypeByte;
	numberOfValues = 1;
	parameterValue = GlobalAlloc(GMEM_FIXED, sizeof(byte));

	if (parameterValue == IntPtr::Zero())
		throw OutOfMemoryException();

	memcpy_s(parameterValue.ToPointer(), sizeof(byte), &value, sizeof(byte));
}

EncoderParameter::~EncoderParameter()
{
	Dispose(false);
}

void EncoderParameter::Dispose()
{
	Dispose(true);
}

void EncoderParameter::SetEncoder(Encoder e)
{
	parameterGuid = e.GetGuid();
}
