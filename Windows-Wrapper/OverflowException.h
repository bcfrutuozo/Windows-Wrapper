#pragma once

#include "ArithmeticException.h"

class OverflowException : public ArithmeticException
{
public:

	OverflowException(int line, const char* file) noexcept;
	OverflowException(int line, const char* file, const std::string& message) noexcept;
	OverflowException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};