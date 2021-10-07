#include "OverflowException.h"

OverflowException::OverflowException(int line, const char* file) noexcept
	:
	ArithmeticException(line, file, "Arg_OverflowException")
{
	SetErrorCode(HResult::COR_E_OVERFLOW);
}

OverflowException::OverflowException(int line, const char* file, const std::string& message) noexcept
	:
	ArithmeticException(line, file, message)
{
	SetErrorCode(HResult::COR_E_OVERFLOW);
}

OverflowException::OverflowException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArithmeticException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_OVERFLOW);
}