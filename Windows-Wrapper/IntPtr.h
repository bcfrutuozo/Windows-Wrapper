#pragma once

#include "IEquatable.h"
#include "IComparable.h"

/*
* Wrapper class for uintptr_t (void*) which provides additional functionalities to handle pointers.
* This class should be used to work along with a teoritically SafeHandle class for handles (HWND).
* 
* However, due to the C++ language, NO GC (GARBAGE COLLECTOR) WILL BE IMPLEMENTED.
* All memory must be managed by the user while using this library! Otherwise he/she MUST use C#
* instead of this wrapper.
* 
* With that in mind, EVERY use of the SafeHandle class in the ReferenceSource API IS TRANSLATED
* AS INTPTR IN THIS PROJECT.
*/
class IntPtr : public IComparable<IntPtr>, public IEquatable<IntPtr>
{
private:

	uintptr_t m_Ptr;

public:

	constexpr IntPtr() : m_Ptr(0L) { }
	constexpr IntPtr(const uintptr_t ptr) : m_Ptr(ptr) {}
	constexpr IntPtr(void* ptr) : m_Ptr(ptr == nullptr ? 0 : (uintptr_t)ptr) {}

	constexpr operator int() { return (int)m_Ptr; }
	constexpr operator void*() { return (void*)m_Ptr; }
	constexpr operator long() { return (long)m_Ptr; }

	constexpr bool operator==(uint64_t p) const noexcept { return m_Ptr == p; }

	constexpr IntPtr& operator=(uint64_t p) noexcept
	{
		m_Ptr = p;
		return *this;
	}

	constexpr bool operator==(const IntPtr& p) const { return m_Ptr == p.m_Ptr; }

	constexpr uint32_t ToInt32() const noexcept
	{
		return static_cast<int>(m_Ptr);
	}

	constexpr uint64_t ToInt64() const noexcept
	{
		return static_cast<long>(m_Ptr);
	}

	constexpr void* ToPointer() const noexcept { return (void*)m_Ptr; }

	constexpr int Size() const noexcept
	{
		return sizeof(uintptr_t);
	}

	constexpr bool IsNull() const noexcept { return m_Ptr == 0; }
	constexpr bool IsValid() const noexcept { return !IsNull(); }

	static constexpr IntPtr Add(IntPtr p, uint32_t offset) noexcept { return IntPtr(p.m_Ptr + offset); }

	constexpr IntPtr& operator=(int p) noexcept
	{
		m_Ptr = static_cast<uintptr_t>(p);
		return *this;
	}

	constexpr IntPtr& operator=(void* p) noexcept
	{
		m_Ptr = p == nullptr ? 0 : (uintptr_t)(p);
		return *this;
	}

	constexpr bool operator==(int p) const noexcept
	{
		return m_Ptr == p;
	}

	constexpr bool operator==(void* p) const noexcept
	{
		return m_Ptr == (p == nullptr ? 0 : (uintptr_t)p);
	}

	constexpr bool operator!=(void* p) const noexcept
	{
		return !(*this == p);
	}

	int GetHashCode() const override;
	int CompareTo(const Object* const obj) const override;
	int CompareTo(const IntPtr* const b) const override;
	bool Equals(const Object* const obj) const override;
	bool Equals(const IntPtr* const p) const override;
	std::string ToString() const noexcept override;

	static constexpr IntPtr Zero() { return IntPtr(static_cast<uintptr_t>(0UL)); }
};