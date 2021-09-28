#pragma once

#include "IEquatable.h"
#include "IComparable.h"

struct IntPtr : public IComparable<IntPtr>, public IEquatable<IntPtr>
{
private:

#ifdef _WIN64
	uint64_t m_Ptr;
#else
	uint32_t m_Ptr;
#endif

public:

#ifdef _WIN64
	constexpr IntPtr(const uint64_t ptr) : m_Ptr(ptr) {}
	constexpr IntPtr(void* ptr) : m_Ptr(ptr == nullptr ? 0 : (uint64_t)ptr) {}

	constexpr bool operator==(uint64_t p) const noexcept { return m_Ptr == p; }

	constexpr IntPtr& operator=(uint64_t p) noexcept
	{
		m_Ptr = p;
		return *this;
	}
#else
	constexpr IntPtr(const uint32_t ptr) : m_Ptr(ptr) {}
	constexpr IntPtr(void* ptr) : m_Ptr(ptr == nullptr ? 0 : (uint32_t)ptr) {}
#endif

	constexpr bool operator==(const IntPtr& p) const { return m_Ptr == p.m_Ptr; }

	constexpr uint32_t ToInt32() const noexcept
	{
#if _WIN64
		return static_cast<uint32_t>(m_Ptr);
#else
		return m_Ptr;
#endif
	}

	constexpr uint64_t ToInt64() const noexcept
	{
#if _WIN64
		return m_Ptr;
#else
		return static_cast<uint64_t>(m_Ptr);
#endif
	}

	constexpr void* ToPointer() const noexcept { return (void*)m_Ptr; }

	constexpr int Size() const noexcept
	{
#if _WIN64
		return 8;
#else
		return 4;
#endif
	}

	constexpr bool IsNull() const noexcept { return m_Ptr == 0; }
	static constexpr IntPtr Add(IntPtr p, uint32_t offset) noexcept { return IntPtr(p.m_Ptr + offset); }

	constexpr IntPtr& operator=(uint32_t p) noexcept
	{
#if _WIN64
		m_Ptr = static_cast<uint64_t>(p);
#else
		m_Ptr = p;
#endif
		return *this;
	}

	constexpr IntPtr& operator=(void* p) noexcept
	{
#if _WIN64
		m_Ptr = p == nullptr ? 0 : (uint64_t)(p);
#else
		m_Ptr = p == nullptr ? 0 : (uint32_t)p;
#endif
		return *this;
	}

	constexpr bool operator==(uint32_t p) const noexcept
	{
#if _WIN64
		return m_Ptr == static_cast<uint64_t>(p);
#else
		return m_Ptr == p;
#endif
	}

	constexpr bool operator==(void* p) const noexcept
	{
#if _WIN64
		return m_Ptr == (p == nullptr ? 0 : (uint64_t)p);
#else
		return m_Ptr == (p == nullptr ? 0 : (uint32_t)p);
#endif
	}

	int GetHashCode() const override;
	int CompareTo(const Object* const obj) const override;
	int CompareTo(const IntPtr* const b) const override;
	bool Equals(const Object* const obj) const override;
	bool Equals(const IntPtr* const p) const override;
	//OVERRIDE OBJECT TO STRING inline virtual const std::string ToString() const noexcept;

#ifdef _WIN64
	static constexpr IntPtr Zero() { return IntPtr(static_cast<uint64_t>(0UL)); }
#else
	static constexpr IntPtr Zero() { return IntPtr(static_cast<uint32_t>(0U)); }
#endif
};