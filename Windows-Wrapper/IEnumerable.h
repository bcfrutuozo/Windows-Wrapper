#pragma once

#include "IEnumerator.h"

template<typename T>
class IEnumerable
{
public:

	virtual IEnumerator<T>* GetEnumerator() const noexcept = 0;
};