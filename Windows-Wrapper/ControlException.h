#pragma once

#include "Exception.h"

class ControlException : public Exception
{
public:

	ControlException(int line, const char* file) noexcept;
	ControlException(int line, const char* file, HRESULT hr) noexcept;
	ControlException(int line, const char* file, HRESULT hr, const std::string& message) noexcept;
};