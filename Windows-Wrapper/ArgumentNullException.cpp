#include "ArgumentNullException.h"

ArgumentNullException::ArgumentNullException(int line, const char* file) noexcept
	:
	ArgumentException(line, file, "Arg_ArgumentNullException")
{
	SetErrorCode(E_POINTER);
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& param) noexcept
	:
	ArgumentException(line, file, "", param)
{
	SetErrorCode(E_POINTER);
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArgumentException(line, file, message, innerException)
{
	SetErrorCode(E_POINTER);
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& message, const std::string& param) noexcept
	:
	ArgumentException(line, file, message, param)
{
	SetErrorCode(E_POINTER);
}
