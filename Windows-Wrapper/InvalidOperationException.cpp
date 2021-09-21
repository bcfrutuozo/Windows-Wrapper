#include "InvalidOperationException.h"

InvalidOperationException::InvalidOperationException(int line, const char* file) noexcept
	:
	SystemException(line, file)
{

}

InvalidOperationException::InvalidOperationException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{

}

InvalidOperationException::InvalidOperationException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{

}