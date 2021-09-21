#include "NotImplementedException.h"

NotImplementedException::NotImplementedException(int line, const char* file) noexcept
	:
	SystemException(line, file)
{

}

NotImplementedException::NotImplementedException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{

}

NotImplementedException::NotImplementedException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{

}