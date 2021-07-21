#pragma once

#include "IntPtr.h"

class IHandle
{
protected:

	~IHandle()
	{
		Handle = nullptr;
	}

public:

	IntPtr Handle = 0;
};