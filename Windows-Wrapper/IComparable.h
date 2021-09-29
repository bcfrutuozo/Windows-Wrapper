#pragma once

#include "CommonObject.h"

template<typename T>
class IComparable : public Object
{
public:

	virtual int CompareTo(const Object* const b) const = 0;
	virtual int CompareTo(const T* const b) const = 0;
};