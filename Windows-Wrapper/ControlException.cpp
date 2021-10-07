#include "ControlException.h"

ControlException::ControlException(int line, const char* file) noexcept
	:
	Exception(line, file, "Arg_ControlException")
{	}

ControlException::ControlException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file)
{
	SetErrorCode(hr);
}

ControlException::ControlException(int line, const char* file, HRESULT hr, const std::string & message) noexcept
	:
	Exception(line, file, message)
{
	SetErrorCode(hr);
}
