#pragma once

#include "Exception.h"

class SystemException : public Exception
{
public:

	SystemException(int line, const char* file) noexcept;
	SystemException(int line, const char* file, const std::string& message) noexcept;
	SystemException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};