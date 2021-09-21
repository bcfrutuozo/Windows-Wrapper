#include "SystemException.h"

// SystemException uses the HRESULT COR_E_SYSTEM, that has the value 0x80131501.

SystemException::SystemException(int line, const char* file) noexcept
	:
	Exception(line, file, 0x80131501)
{	}

SystemException::SystemException(int line, const char* file, const std::string& message) noexcept
	:
	Exception(line, file, 0x80131501, message)
{	}

SystemException::SystemException(int line, const char* file, const std::string& message, Exception* const innerException) noexcept
	:
	Exception(line, file, 0x80131501, message, innerException)
{	}