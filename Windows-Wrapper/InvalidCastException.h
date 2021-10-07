#pragma once

#include "SystemException.h"

class InvalidCastException : public SystemException
{
public:

	InvalidCastException(int line, const char* file) noexcept;
	InvalidCastException(int line, const char* file, const std::string& message) noexcept;
	InvalidCastException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	InvalidCastException(int line, const char* file, const std::string& message, HRESULT hr) noexcept;
};