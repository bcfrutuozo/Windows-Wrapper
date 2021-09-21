#pragma once

#include "ArgumentException.h"

#include <string>

class ArgumentNullException : public ArgumentException
{
public:

	ArgumentNullException(int line, const char* file) noexcept;
	ArgumentNullException(int line, const char* file, const std::string& param) noexcept;
	ArgumentNullException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	ArgumentNullException(int line, const char* file, const std::string& message, const std::string& param) noexcept;
};