#pragma once

#include "Object.h"
#include "IEnumerator.h"

class Enumerator : public IEnumerator, public Object
{
	friend class Iterator;
	friend class ConstIterator;

private:

	Object* Item;

public:

	Enumerator* Begin;
	Enumerator* Prior;
	Enumerator* Next;

	Enumerator(Object* const item);
	Enumerator(Object* const item, Enumerator* const prior);
	Enumerator(Object* const item, Enumerator* const prior, Enumerator* const next);
	Enumerator(const Enumerator& src);
	~Enumerator();

	//Enumerator& operator=(const Enumerator& obj) = delete;

	Object* GetCurrent() const noexcept override;
	bool MoveNext() noexcept override;
	bool MovePrior() noexcept override;
	void Reset() noexcept;
};