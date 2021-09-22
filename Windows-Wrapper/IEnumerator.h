#pragma once

#include "Object.h"

template<typename T>
class IEnumerator
{
public:

	virtual T* GetCurrent() const noexcept = 0;
};