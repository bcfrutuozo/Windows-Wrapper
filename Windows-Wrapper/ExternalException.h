#pragma once

#include "SystemException.h"

class ExternalException : public SystemException
{
public:

	ExternalException(int line, const char* file) noexcept;
	ExternalException(int line, const char* file, const std::string& message) noexcept;
	ExternalException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	ExternalException(int line, const char* file, const std::string& message, int errorCode) noexcept;
};