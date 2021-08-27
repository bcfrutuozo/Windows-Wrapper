#pragma once

#include "IEnumerator.h"

class IEnumerable
{
public:

	virtual IEnumerator* GetEnumerator() const noexcept = 0;
};