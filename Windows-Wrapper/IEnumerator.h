#pragma once

#include "Object.h"

class IEnumerator
{
public:

	virtual Object* GetCurrent() const noexcept = 0;
	virtual bool MoveNext() noexcept = 0;
	virtual bool MovePrior() noexcept = 0;
	virtual void Reset() noexcept = 0;
};