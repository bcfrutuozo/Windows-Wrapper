#pragma once

#include "SystemException.h"

class InvalidOperationException : public SystemException
{
public:

	InvalidOperationException(int line, const char* file) noexcept;
	InvalidOperationException(int line, const char* file, const std::string& message) noexcept;
	InvalidOperationException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};