#include "ArgumentException.h"

ArgumentException::ArgumentException(int line, const char* file) noexcept
	:
	SystemException(line, file)
{	}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{	}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{	}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, const std::string& param) noexcept
	:
	SystemException(line, file, message),
	m_Param(param)
{	}

ArgumentException::ArgumentException(int line, const char* file, const std::string& message, const std::string& param, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException),
	m_Param(param)
{	}

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