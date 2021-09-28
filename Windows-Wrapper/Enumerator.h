#pragma once

#include "Object.h"
#include "IEnumerator.h"

template<typename T>
class Enumerator : public IEnumerator<T>
{
	friend class Iterator;
	friend class ConstIterator;

private:

	T* Item;
	int Index = -1;

public:

	Enumerator<T>* Begin;
	Enumerator<T>* Prior;
	Enumerator<T>* Next;

	// Constructor called when the Enumerator is the first one on Collection
	Enumerator(T* const item)
		:
		Enumerator(item, nullptr, nullptr)
	{
		// Overrite Begin pointer because it's the first Enumerator
		Begin = this;
		Index = 0;
	}

	Enumerator(T* const item, Enumerator<T>* const prior)
		:
		Enumerator(item, prior, nullptr)
	{	}

	Enumerator(T* const item, Enumerator<T>* const prior, Enumerator<T>* const next)
		:
		Item(item),
		Begin(prior == nullptr ? nullptr : prior->Begin),
		Prior(prior),
		Next(next),
		Index(prior == nullptr ? 0 : prior->Index + 1)
	{	}

	Enumerator(const Enumerator<T>& src)
	{
		Item = src.Item;
		Begin = src.Begin;
		Next = src.Next;
		Prior = src.Prior;
		Index = src.Index;
	}

	virtual ~Enumerator()
	{
		if (Begin != nullptr)
		{
			Begin = nullptr;
		}

		if (Prior != nullptr)
		{
			Prior = nullptr;
		}
		if (Next != nullptr)
		{
			Next = nullptr;
		}
	}

	T* GetCurrent() const noexcept override
	{
		return Item;
	}
};