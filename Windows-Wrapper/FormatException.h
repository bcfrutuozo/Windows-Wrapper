#pragma once

#include "SystemException.h"

class FormatException : public SystemException
{
public:

	FormatException(int line, const char* file) noexcept;
	FormatException(int line, const char* file, const std::string& message) noexcept;
	FormatException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};
