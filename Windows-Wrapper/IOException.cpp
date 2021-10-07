#include "IOException.h"

IOException::IOException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_IOException")
{
	SetErrorCode(HResult::COR_E_IO);
}

IOException::IOException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_IO);
}

IOException::IOException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_IO);
}

IOException::IOException(int line, const char* file, const std::string& message, HRESULT hr) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(hr);
}
