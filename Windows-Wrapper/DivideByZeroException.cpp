#include "DivideByZeroException.h"

DivideByZeroException::DivideByZeroException(int line, const char* file) noexcept
	:
	ArithmeticException(line, file)
{

}

DivideByZeroException::DivideByZeroException(int line, const char* file, const std::string& message) noexcept
	:
	ArithmeticException(line, file, message)
{

}

DivideByZeroException::DivideByZeroException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArithmeticException(line, file, message, innerException)
{

}