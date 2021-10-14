#pragma once

#include "ExternalException.h"

class Win32Exception : public ExternalException
{
private:

	const int m_ErrorCode;

public:

	Win32Exception(int line, const char* file) noexcept;
	Win32Exception(int line, const char* file, int errorCode) noexcept;
	Win32Exception(int line, const char* file, int errorCode, const std::string& message) noexcept;
	Win32Exception(int line, const char* file, const std::string& message) noexcept;
	Win32Exception(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
};