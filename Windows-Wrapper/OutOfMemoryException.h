#pragma once

#include "SystemException.h"

class OutOfMemoryException : public SystemException
{
public:

	OutOfMemoryException(int line, const char* file) noexcept;
	OutOfMemoryException(int line, const char* file, const std::string& message) noexcept;
	OutOfMemoryException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};