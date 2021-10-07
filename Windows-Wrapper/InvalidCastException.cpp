#include "InvalidCastException.h"

InvalidCastException::InvalidCastException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_InvalidCastException")
{
	SetErrorCode(HResult::COR_E_INVALIDCAST);
}

InvalidCastException::InvalidCastException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_INVALIDCAST);
}

InvalidCastException::InvalidCastException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_INVALIDCAST);
}

InvalidCastException::InvalidCastException(int line, const char* file, const std::string& message, HRESULT hr) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(hr);
}
