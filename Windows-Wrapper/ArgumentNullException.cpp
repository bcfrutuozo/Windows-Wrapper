#include "ArgumentNullException.h"

ArgumentNullException::ArgumentNullException(int line, const char* file) noexcept
	:
	ArgumentException(line, file)
{
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& param) noexcept
	:
	ArgumentException(line, file, "", param)
{
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArgumentException(line, file, message, innerException)
{
}

ArgumentNullException::ArgumentNullException(int line, const char* file, const std::string& message, const std::string& param) noexcept
	:
	ArgumentException(line, file, message, param)
{
}
