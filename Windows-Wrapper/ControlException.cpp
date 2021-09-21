#include "ControlException.h"

ControlException::ControlException(int line, const char* file) noexcept
	:
	Exception(line, file)
{	}

ControlException::ControlException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file, hr)
{	}

ControlException::ControlException(int line, const char* file, HRESULT hr, const std::string & message) noexcept
	:
	Exception(line, file, hr, message)
{
}
