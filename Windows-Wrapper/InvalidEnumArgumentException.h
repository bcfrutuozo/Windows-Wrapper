#pragma once

#include "ArgumentException.h"

class InvalidEnumArgumentException : public ArgumentException
{
public:

	InvalidEnumArgumentException(int line, const char* file) noexcept;
	InvalidEnumArgumentException(int line, const char* file, const std::string& message) noexcept;
	InvalidEnumArgumentException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	InvalidEnumArgumentException(int line, const char* file, const std::string& argumentName, int invalidValue, const std::string& enumTypeClassName) noexcept;
};
