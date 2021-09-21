#pragma once

#include "SystemException.h"

class NotImplementedException : public SystemException
{
public:

	NotImplementedException(int line, const char* file) noexcept;
	NotImplementedException(int line, const char* file, const std::string& message) noexcept;
	NotImplementedException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};

