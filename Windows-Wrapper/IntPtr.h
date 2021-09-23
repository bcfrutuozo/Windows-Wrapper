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
#else
	constexpr IntPtr(const uint32_t ptr) : m_Ptr(ptr) {}
	constexpr IntPtr(void* ptr) : m_Ptr(ptr == nullptr ? 0 : (uint32_t)ptr) {}
#endif

	bool operator==(const IntPtr& p) const;
	int CompareTo(const Object* const obj) const override;
	int CompareTo(const IntPtr* const b) const override;
	bool Equals(const Object* const obj) const override;
	bool Equals(const IntPtr* const p) const override;

	uint32_t ToInt32() const noexcept;
	uint64_t ToInt64() const noexcept;
	void* ToPointer() const noexcept;
	int Size() const noexcept;
	bool IsNull() const noexcept;
	inline static IntPtr Add(IntPtr p, uint32_t offset) noexcept;
	IntPtr& operator=(uint32_t p) noexcept;
	IntPtr& operator=(void* p) noexcept;
	bool operator==(uint32_t p) const noexcept;
	bool operator==(void* p) const noexcept;

#ifdef _WIN64
	IntPtr& operator=(uint64_t p) noexcept;
	bool operator==(uint64_t p) const noexcept;

	static constexpr IntPtr Zero() { return IntPtr(static_cast<uint64_t>(0UL)); }
#else
	static constexpr IntPtr Zero() { return IntPtr(static_cast<uint32_t>(0U)); }
#endif
};