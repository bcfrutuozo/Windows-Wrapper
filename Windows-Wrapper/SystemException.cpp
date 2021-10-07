#include "SystemException.h"

SystemException::SystemException(int line, const char* file) noexcept
	:
	Exception(line, file, "Arg_SystemException")
{
	SetErrorCode(HResult::COR_E_SYSTEM);
}

SystemException::SystemException(int line, const char* file, const std::string& message) noexcept
	:
	Exception(line, file, message)
{
	SetErrorCode(HResult::COR_E_SYSTEM);
}

SystemException::SystemException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	Exception(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_SYSTEM);
}