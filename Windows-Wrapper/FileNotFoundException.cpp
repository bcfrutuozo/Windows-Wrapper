#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(int line, const char* file) noexcept
	:
	IOException(line, file, "Arg_FileNotFoundException")
{
	SetErrorCode(HResult::COR_E_FILENOTFOUND);
}

FileNotFoundException::FileNotFoundException(int line, const char* file, const std::string& message) noexcept
	:
	IOException(line, file, message)
{
	SetErrorCode(HResult::COR_E_FILENOTFOUND);
}

FileNotFoundException::FileNotFoundException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	IOException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_FILENOTFOUND);
}

FileNotFoundException::FileNotFoundException(int line, const char* file, const std::string& message, const std::string& filename) noexcept
	:
	IOException(line, file, message),
	m_Filename(filename)
{
	SetErrorCode(HResult::COR_E_FILENOTFOUND);
	SetMessage("Could not load file '" + m_Filename + "'");
}
FileNotFoundException::FileNotFoundException(int line, const char* file, const std::string& message, const std::string& filename, Exception* const innerException) noexcept
	:
	IOException(line, file, message, innerException),
	m_Filename(filename)
{
	SetErrorCode(HResult::COR_E_FILENOTFOUND);
	SetMessage("Could not load file '" + m_Filename + "'");
}
