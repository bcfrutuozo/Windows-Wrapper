#pragma once

#include "IEnumerator.h"

template<typename T = Object>
class IEnumerable : public Object
{
public:

	virtual IEnumerator<T>* GetEnumerator() const noexcept = 0;
};