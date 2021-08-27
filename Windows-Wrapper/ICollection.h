#pragma once

#include "IEnumerable.h"

template<typename T>
class ICollection : public IEnumerable
{
protected:

	size_t Count;

public:

	virtual void Add(T* const item) noexcept = 0;
	virtual bool Remove(T* const item) noexcept = 0;
	virtual void Clear() noexcept = 0;
	virtual bool Contains(T* const item) const noexcept = 0;
	
	size_t GetCount() const noexcept
	{
		return Count;
	}
	
	bool IsEmpty() const noexcept
	{
		return Count == 0;
	}
};