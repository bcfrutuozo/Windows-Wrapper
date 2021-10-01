#pragma once

#include "SystemException.h"

class ArithmeticException : public SystemException
{
public:

	ArithmeticException(int line, const char* file) noexcept;
	ArithmeticException(int line, const char* file, const std::string& message) noexcept;
	ArithmeticException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};