#pragma once

#include "Object.h"
#include "ICollection.h"
#include "Enumerator.h"

#include <cassert>

template<typename T>
class Collection : public ICollection<T>, public Object
{
private:

	T* Owner;

protected:

	Enumerator* pNext;

public:

	IEnumerator* GetEnumerator() const noexcept override
	{
		return pNext;
	}

	Collection()
		:
		Owner(nullptr),
		pNext(nullptr)
	{ }

	Collection(T* owner)
		:
		Owner(owner),
		pNext(nullptr)
	{ }

	Collection(const Collection& src)
	{
		*this = src;
	}

	Collection& operator=(const Collection& src)
	{
		if (&src != this)
		{
			if (!this->IsEmpty())
			{
				delete pNext;
				pNext = nullptr;
			}

			if (!src.IsEmpty())
			{
				pNext = new Enumerator(*src.pNext);
			}
		}
		return *this;
	}

	T* operator[](size_t i) const noexcept
	{
		assert(i < this->Count);

		if (i == 0)
		{
			return dynamic_cast<T*>(pNext->GetCurrent());
		}

		auto e = pNext->Begin;
		for (size_t p = 0; p < i; ++p, e = e->Next)
		{
			if (p == i)
			{
				return dynamic_cast<T*>(e->GetCurrent());
			}
		}
	}

	virtual ~Collection()
	{
		if (pNext != nullptr)
		{
			delete pNext;
			pNext = nullptr;
		}
	}

	void Add(T* const item) noexcept override
	{
		if (this->Count == 0)
		{
			pNext = new Enumerator(item);
		}
		else
		{
			auto p = new Enumerator(item, pNext, nullptr);
			pNext->Next = p;
			pNext = p;
		}

		++this->Count;
	}

	bool Remove(T* const item) noexcept override
	{
		Enumerator* search = pNext;

		while (search != nullptr)
		{
			if (search->GetCurrent() == item)
			{
				Enumerator* temp = search;
				Enumerator* next = search->Next;
				Enumerator* prior = search->Prior;

				if (this->Count > 1)
				{
					if (next == nullptr)
					{
						temp->Prior->Next = nullptr;
						pNext = prior;	// Change the end of the Collection to the new last element
					}
					else
					{
						next->Prior = prior;
					}
					if (prior == nullptr)
					{
						temp->Next->Prior = nullptr;
					}
					else
					{
						prior->Next = next;
					}
				}

				--this->Count;

				T* c = dynamic_cast<T*>(temp->GetCurrent());

				return true;
			}
			else
			{
				search = search->Prior;
			}
		}

		return false;
	}

	void Clear() noexcept override
	{
		if (this->Count == 0)
		{
			return;
		}

		Enumerator* search = pNext;

		while (search != nullptr)
		{
			Enumerator* prior = search->Prior;
			if (prior != nullptr)
			{
				pNext = prior;
				pNext->Next = nullptr;
			}

			delete search;
			search = pNext;
			--this->Count;
		}

		search = nullptr;
	}

	bool Contains(T* const item) const noexcept override
	{
		Enumerator* search = pNext;

		while (search != nullptr)
		{
			if (search->GetCurrent() == item)
			{
				return true;
			}
		}

		return false;
	}

	class Iterator
	{
	private:

		Enumerator* pElement = nullptr;

	public:

		Iterator() = default;
		Iterator(Enumerator* pElement)
			:
			pElement(pElement == nullptr ? nullptr : pElement->Begin)
		{

		}

		~Iterator()
		{
			pElement = nullptr;
		}

		Iterator& operator++()
		{
			pElement = pElement->Next;
			return *this;
		}
		T* operator*()
		{
			if (pElement != nullptr)
			{
				return dynamic_cast<T*>(pElement->GetCurrent());
			}

			return nullptr;
		}
		bool operator!=(Iterator rhs)
		{
			return pElement != rhs.pElement;
		}
	};

	class ConstIterator
	{
	private:

		const Enumerator* pElement = nullptr;

	public:

		ConstIterator() = default;
		ConstIterator(const Enumerator* pElement)
			:
			pElement(pElement == nullptr ? nullptr : pElement->Begin)
		{

		}

		~ConstIterator()
		{
			pElement = nullptr;
		}

		ConstIterator& operator++()
		{
			pElement = pElement->Next;
			return *this;
		}
		const T* operator*() const
		{
			if (pElement != nullptr)
			{
				return dynamic_cast<T*>(pElement->GetCurrent());
			}

			return nullptr;
		}
		bool operator!=(ConstIterator rhs) const
		{
			return pElement != rhs.pElement;
		}
	};

	Iterator begin() { return{ pNext }; }
	Iterator end() { return{}; }
	ConstIterator begin() const { return{ pNext }; }
	ConstIterator end() const { return{}; }
};