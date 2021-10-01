#pragma once

#include "ArithmeticException.h"

class DivideByZeroException : public ArithmeticException
{
public:

	DivideByZeroException(int line, const char* file) noexcept;
	DivideByZeroException(int line, const char* file, const std::string& message) noexcept;
	DivideByZeroException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};