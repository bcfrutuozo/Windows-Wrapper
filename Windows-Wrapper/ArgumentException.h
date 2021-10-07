#pragma once

#include "SystemException.h"

class ArgumentException : public SystemException
{
private:

	std::string m_Param;

public:

	ArgumentException(int line, const char* file) noexcept;
	ArgumentException(int line, const char* file, const std::string& message) noexcept;
	ArgumentException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	ArgumentException(int line, const char* file, const std::string& message, const std::string& param) noexcept;
	ArgumentException(int line, const char* file, const std::string& message, const std::string& param, Exception* const innerException) noexcept;

	const char* what() const noexcept override;
};
