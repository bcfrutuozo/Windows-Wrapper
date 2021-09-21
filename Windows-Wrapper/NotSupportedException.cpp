#include "NotSupportedException.h"

NotSupportedException::NotSupportedException(int line, const char* file) noexcept
	:
	SystemException(line, file)
{

}

NotSupportedException::NotSupportedException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{

}

NotSupportedException::NotSupportedException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{

}