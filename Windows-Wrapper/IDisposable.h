#pragma once

#include "CommonObject.h"

class IDisposable : public Object
{
public:

	virtual void Dispose() = 0;
};