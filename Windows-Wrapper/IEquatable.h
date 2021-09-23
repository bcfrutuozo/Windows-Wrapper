#pragma once

#include "Object.h"

template<class T>
class IEquatable : public Object
{
	virtual bool Equals(const T* const b) const = 0;
};