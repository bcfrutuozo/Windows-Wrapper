#pragma once

#include "IOException.h"

class FileNotFoundException : public IOException
{
private:

	std::string m_Filename;

public:

	FileNotFoundException(int line, const char* file) noexcept;
	FileNotFoundException(int line, const char* file, const std::string& message) noexcept;
	FileNotFoundException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept;
	FileNotFoundException(int line, const char* file, const std::string& message, const std::string& filename) noexcept;
	FileNotFoundException(int line, const char* file, const std::string& message, const std::string& filename, Exception* const innerException) noexcept;
};

