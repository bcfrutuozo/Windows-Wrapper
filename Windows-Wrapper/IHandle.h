#pragma once

#include "IntPtr.h"

class IHandle
{
public:

	IntPtr Handle = 0;

	virtual const IntPtr GetHandle() const noexcept
	{
		return Handle;
	}
};