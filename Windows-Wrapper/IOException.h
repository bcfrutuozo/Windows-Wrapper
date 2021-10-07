#pragma once

#include "SystemException.h"

class IOException : public SystemException
{
public:

	IOException(int line, const char* file) noexcept;
	IOException(int line, const char* file, const std::string& message) noexcept;
	IOException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	IOException(int line, const char* file, const std::string& message, HRESULT hr) noexcept;
};
