#include "DivideByZeroException.h"

DivideByZeroException::DivideByZeroException(int line, const char* file) noexcept
	:
	ArithmeticException(line, file, "Arg_DivideByZeroException")
{
	SetErrorCode(HResult::COR_E_DIVIDEBYZERO);
}

DivideByZeroException::DivideByZeroException(int line, const char* file, const std::string& message) noexcept
	:
	ArithmeticException(line, file, message)
{
	SetErrorCode(HResult::COR_E_DIVIDEBYZERO);
}

DivideByZeroException::DivideByZeroException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArithmeticException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_DIVIDEBYZERO);
}