#include "NotImplementedException.h"

NotImplementedException::NotImplementedException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_NotImplementedException")
{
	SetErrorCode(E_NOTIMPL);
}

NotImplementedException::NotImplementedException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(E_NOTIMPL);
}

NotImplementedException::NotImplementedException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(E_NOTIMPL);
}