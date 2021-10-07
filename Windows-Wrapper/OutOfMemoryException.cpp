#include "OutOfMemoryException.h"

OutOfMemoryException::OutOfMemoryException(int line, const char* file) noexcept
	:
	SystemException(line, file, "Arg_OutOfMemoryException")
{
	SetErrorCode(HResult::COR_E_OUTOFMEMORY);
}

OutOfMemoryException::OutOfMemoryException(int line, const char* file, const std::string& message) noexcept
	:
	SystemException(line, file, message)
{
	SetErrorCode(HResult::COR_E_OUTOFMEMORY);
}

OutOfMemoryException::OutOfMemoryException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	SystemException(line, file, message, innerException)
{
	SetErrorCode(HResult::COR_E_OUTOFMEMORY);
}