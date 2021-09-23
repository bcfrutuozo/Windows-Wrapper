#pragma once

#include "IEnumerator.h"
#include "Exceptions.h"

template<typename T>
class IEnumerable : public Object
{
public:

	virtual IEnumerator<T>* GetEnumerator() const noexcept = 0;
};