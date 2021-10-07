#include "ArgumentOutOfRangeException.h"

#include <sstream>

ArgumentOutOfRangeException::ArgumentOutOfRangeException(int line, const char* file) noexcept
	:
	ArgumentException(line, file, "Arg_ArgumentOutOfRangeException"),
	m_Value(nullptr)
{
	SetErrorCode(HResult::COR_E_ARGUMENTOUTOFRANGE);
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(int line, const char* file, const std::string& param) noexcept
	:
	ArgumentException(line, file, "", param),
	m_Value(nullptr)
{
	SetErrorCode(HResult::COR_E_ARGUMENTOUTOFRANGE);
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	ArgumentException(line, file, message, innerException),
	m_Value(nullptr)
{
	SetErrorCode(HResult::COR_E_ARGUMENTOUTOFRANGE);
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(int line, const char* file, const std::string& param, Object* value, const std::string& message) noexcept
	:
	ArgumentException(line, file, message, param),
	m_Value(value)
{
	SetErrorCode(HResult::COR_E_ARGUMENTOUTOFRANGE);
}

ArgumentOutOfRangeException::ArgumentOutOfRangeException(int line, const char* file, const std::string& param, const std::string& message) noexcept
	:
	ArgumentException(line, file, message, param),
	m_Value(nullptr)
{
	SetErrorCode(HResult::COR_E_ARGUMENTOUTOFRANGE);
}

const char* ArgumentOutOfRangeException::what() const noexcept
{
	std::ostringstream oss;

	if (m_Value != nullptr)
	{
		oss << "Value: " << m_Value->ToString() << std::endl << ArgumentException::what();
		m_WhatBuffer = oss.str();
	}
	else
	{
		ArgumentException::what();
	}

	return m_WhatBuffer.c_str();
}