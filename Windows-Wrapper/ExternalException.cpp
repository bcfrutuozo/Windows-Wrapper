#include "ExternalException.h"

ExternalException::ExternalException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_ExternalException")
{
	SetErrorCode(E_FAIL);
}

ExternalException::ExternalException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(E_FAIL);
}

ExternalException::ExternalException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(E_FAIL);
}

ExternalException::ExternalException(int line, const char* file, const std::string& message, int errorCode) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(errorCode);
}
