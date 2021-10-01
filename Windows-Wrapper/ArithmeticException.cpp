#include "ArithmeticException.h"

ArithmeticException::ArithmeticException(int line, const char* file) noexcept
	:
	SystemException(line, file)
{

}

ArithmeticException::ArithmeticException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{

}

ArithmeticException::ArithmeticException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{

}