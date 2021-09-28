#pragma once

#include "ICollection.h"

template<typename T>
class IList : public ICollection<T>
{
public:

	virtual T* operator[](size_t index) const = 0;
	virtual int IndexOf(T* item) const noexcept = 0;
	virtual void Insert(size_t index, T* item) = 0;
	virtual bool IsReadOnly() const noexcept = 0;
	virtual bool RemoveAt(size_t index) = 0;
};