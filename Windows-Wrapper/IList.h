#pragma once

#include "ICollection.h"

template<typename T>
class IList : public ICollection<T>
{
public:

	virtual T* operator[](size_t index) const = 0;
	virtual int IndexOf(T* item) const noexcept = 0;
	virtual void Insert(int index, T* item) noexcept = 0;
	virtual bool IsReadOnly() const noexcept = 0;
	virtual bool RemoveAt(int index) = 0;
};