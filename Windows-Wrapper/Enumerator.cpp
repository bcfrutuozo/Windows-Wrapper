#include "Enumerator.h"

// Constructor called when the Enumerator is the first one on Collection
Enumerator::Enumerator(Object* item)
	:
	Enumerator(item, nullptr, nullptr)
{
	// Overrite Begin pointer because it's the first Enumerator
	Begin = this;
}

Enumerator::Enumerator(Object* const item, Enumerator* const prior)
	:
	Enumerator(item, prior, nullptr)
{

}

Enumerator::Enumerator(Object* const item, Enumerator* const prior, Enumerator* const next)
	:
	Item(item),
	Begin(prior == nullptr ? nullptr : prior->Begin),
	Prior(prior),
	Next(next)
{

}

Enumerator::Enumerator(const Enumerator& src)
{
	Item = src.Item;
	Begin = src.Begin;
	Next = new Enumerator(*src.Next);
	Prior = new Enumerator(*src.Prior);
}

Enumerator::~Enumerator()
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

//Enumerator& operator=(const Enumerator& obj) = delete;

Object* Enumerator::GetCurrent() const noexcept
{
	return Item;
}

bool Enumerator::MoveNext() noexcept
{
	if (Next != nullptr)
	{
		*this = Next;
		return true;
	}

	return false;
}

bool Enumerator::MovePrior() noexcept
{
	if (Prior != nullptr)
	{
		*this = Prior;
		return true;
	}

	return false;
}

void Enumerator::Reset() noexcept
{
	*this = Begin;
}