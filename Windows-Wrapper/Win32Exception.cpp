#include "Win32Exception.h"

Win32Exception::Win32Exception(int line, const char* file) noexcept
	:
	Win32Exception(line, file, GetLastError())
{
	
}

Win32Exception::Win32Exception(int line, const char* file, int errorCode) noexcept
	:
	Win32Exception(line, file, errorCode, TranslateErrorCode(errorCode))
{

}

Win32Exception::Win32Exception(int line, const char* file, int errorCode, const std::string& message) noexcept
	:
	ExternalException(line, file, message),
	m_ErrorCode(errorCode)
{

}

Win32Exception::Win32Exception(int line, const char* file, const std::string& message) noexcept
	:
	Win32Exception(line, file, GetLastError(), message)
{

}

Win32Exception::Win32Exception(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ExternalException(line, file, message, innerException),
	m_ErrorCode(GetLastError())
{

}