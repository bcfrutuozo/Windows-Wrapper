#pragma once

#include "ArgumentException.h"

class ArgumentOutOfRangeException : public ArgumentException
{
private:

	Object* m_Value;

public:

	ArgumentOutOfRangeException(int line, const char* file) noexcept;
	ArgumentOutOfRangeException(int line, const char* file, const std::string& param) noexcept;
	ArgumentOutOfRangeException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	ArgumentOutOfRangeException(int line, const char* file, const std::string& param, Object* value, const std::string& message) noexcept;
	ArgumentOutOfRangeException(int line, const char* file, const std::string& param, const std::string& message) noexcept;
	
	const char* what() const noexcept override;
};

