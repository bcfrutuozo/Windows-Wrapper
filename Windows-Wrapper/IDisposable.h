#pragma once

#include "Object.h"

class IDisposable : public Object
{
public:

	virtual void Dispose() = 0;
};