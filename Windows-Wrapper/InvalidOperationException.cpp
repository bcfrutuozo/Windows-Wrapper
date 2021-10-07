#include "InvalidOperationException.h"

InvalidOperationException::InvalidOperationException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_InvalidOperationException")
{
	SetErrorCode(HResult::COR_E_INVALIDOPERATION);
}

InvalidOperationException::InvalidOperationException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_INVALIDOPERATION);
}

InvalidOperationException::InvalidOperationException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_INVALIDOPERATION);
}