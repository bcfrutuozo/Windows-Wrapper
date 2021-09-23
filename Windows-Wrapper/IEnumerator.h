#pragma once

#include "Object.h"

template<typename T>
class IEnumerator : public Object
{
public:

	virtual T* GetCurrent() const noexcept = 0;
};