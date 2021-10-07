#include "ArgumentException.h"
#include <sstream>

ArgumentException::ArgumentException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_ArgumentException")
{
	SetErrorCode(HResult::COR_E_ARGUMENT);
}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_ARGUMENT);
}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_ARGUMENT);
}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, const std::string& param) noexcept
	:
	SystemException(line, file, message),
	m_Param(param)
{
	SetErrorCode(HResult::COR_E_ARGUMENT);
}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, const std::string& param, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException),
	m_Param(param)
{
	SetErrorCode(HResult::COR_E_ARGUMENT);
}

const char* ArgumentException::what() const noexcept
{
	std::ostringstream oss;

	if (!m_Param.empty())
	{
		oss << "Param: " << m_Param << std::endl << SystemException::what();
		m_WhatBuffer = oss.str();
	}
	else
	{
		SystemException::what();
	}

	return m_WhatBuffer.c_str();
}