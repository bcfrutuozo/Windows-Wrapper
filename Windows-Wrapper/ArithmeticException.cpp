#include "ArithmeticException.h"

ArithmeticException::ArithmeticException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_ArithmeticException")
{
	SetErrorCode(HResult::COR_E_ARITHMETIC);
}

ArithmeticException::ArithmeticException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_ARITHMETIC);
}

ArithmeticException::ArithmeticException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_ARITHMETIC);
}