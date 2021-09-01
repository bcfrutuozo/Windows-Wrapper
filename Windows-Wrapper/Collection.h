#pragma once

#include "Object.h"
#include "IList.h"
#include "Enumerator.h"

#include <cassert>

template<typename T>
class Collection : public IList<T>, public Object
{
private:

	T* Owner;
	bool m_ReadOnly;

	void ResetIndices(Enumerator* begin) noexcept
	{
		auto start = begin;
		while (start != nullptr)
		{
			start->Begin = begin;
			start = start->Next;
		}
	}

	bool Remove(Enumerator* e) noexcept
	{
		Enumerator* temp = e;
		Enumerator* next = e->Next;
		Enumerator* prior = e->Prior;

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

		delete dynamic_cast<T*>(temp->GetCurrent());

		if (temp == temp->Begin)
		{
			ResetIndices(temp->Next);
		}

		delete temp;

		return true;
	}

	class Iterator
	{
	private:

		Enumerator* pElement = nullptr;

	public:

		Iterator() = default;
		Iterator(Enumerator* pElement)
			:
			pElement(pElement)
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
			pElement(pElement)
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

protected:

	Enumerator* pNext;

public:

	Iterator begin() { return{ this->Count == 0 ? nullptr : pNext->Begin }; }
	Iterator end() { return{}; }
	ConstIterator begin() const { return{ this->Count == 0 ? nullptr : pNext->Begin }; }
	ConstIterator end() const { return{}; }

	IEnumerator* GetEnumerator() const noexcept override
	{
		return pNext;
	}

	Collection()
		:
		Collection(nullptr)
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

	T* operator[](size_t index) const override
	{
		if (index >= this->Count)
		{
			throw std::out_of_range("Invalid Enumerator range");
		}

		auto e = pNext->Begin;
		T* r = nullptr;

		for (size_t i = 0; i < index; ++i, e = e->Next)
		{
			if (i == index)
			{
				r = dynamic_cast<T*>(e->GetCurrent());
				break;
			}
		}

		return r;
	}

	virtual ~Collection()
	{
		if (Owner != nullptr)
		{
			Owner = nullptr;
		}

		Clear();
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
				return Remove(search);
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

		while (pNext != nullptr)
		{
			Enumerator* temp = nullptr;
			if (pNext->Prior != nullptr)
			{
				temp = pNext->Prior;
			}

			Remove(pNext);
			pNext = temp;
		}
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

	int IndexOf(T* item) const noexcept override
	{
		int index = 0;

		for (auto it = begin(); it != end(); ++it, ++index)
		{
			if (*it == item)
			{
				break;
			}
		}

		return index;
	}

	void Insert(int index, T* item) noexcept override
	{
		if (index < 0 || index >= this->Count)
		{
			throw std::out_of_range("Invalid index");
		}

		auto e = pNext->Begin;
		Enumerator* prior = nullptr;
		Enumerator* next = nullptr;

		for (size_t i = 0; i < index; ++i, e = e->Next)
		{
			if (i == index)
			{
				prior = e;
			}

			if (i == index + 1)
			{
				next = e;
				break;
			}
		}

		auto n = new Enumerator(item, prior, next);
		prior->Next = n;
		next->Prior = n;

		++this->Count;
	}

	bool IsReadOnly() const noexcept override
	{
		return m_ReadOnly;
	}

	bool RemoveAt(int index) override
	{
		if (index >= this->Count)
		{
			throw std::out_of_range("Invalid index");
		}

		int i = 0;
		for (auto it = begin(); it != end(); ++it, ++i)
		{
			if (index == i)
			{
				return Remove(*it);
			}
		}
	}
};