#pragma once

#include "SystemException.h"

class NotSupportedException : public SystemException
{
public:

	NotSupportedException(int line, const char* file) noexcept;
	NotSupportedException(int line, const char* file, const std::string& message) noexcept;
	NotSupportedException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};
