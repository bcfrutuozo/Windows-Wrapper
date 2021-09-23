#include "IntPtr.h"
#include "Exceptions.h"

bool IntPtr::operator==(const IntPtr& p) const
{
	return m_Ptr == p.m_Ptr;
}

int IntPtr::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto ptr = dynamic_cast<const IntPtr*>(obj))
	{
		if (m_Ptr < ptr->m_Ptr) return -1;
		if (m_Ptr > ptr->m_Ptr) return 1;

		return 0;
	}

	throw ArgumentException("Arg_MustBeIntPtr");
}

int IntPtr::CompareTo(const IntPtr* const b) const
{
	if (b == nullptr) return 1;

	if (m_Ptr < b->m_Ptr) return -1;
	if (m_Ptr > b->m_Ptr) return 1;

	return 0;
}

bool IntPtr::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto pad = dynamic_cast<const IntPtr*>(obj))
	{
		return *this == *pad;
	}

	throw ArgumentException("Arg_MustBeIntPtr");
}

bool IntPtr::Equals(const IntPtr* const p) const
{
	if (p == nullptr) return false;
	return *this == *p;
}

// Remove compile warnings regarding pointer conversions
#pragma warning(push)
#pragma warning(disable : 4244)

uint32_t IntPtr::ToInt32() const noexcept
{
#if _WIN64
	return static_cast<uint32_t>(m_Ptr);
#else
	return m_Ptr;
#endif
}

uint64_t IntPtr::ToInt64() const noexcept
{
#if _WIN64
	return m_Ptr;
#else
	return static_cast<uint64_t>(m_Ptr);
#endif
}

#pragma warning(pop)

void* IntPtr::ToPointer() const noexcept
{
	return (void*)(m_Ptr);
}

int IntPtr::Size() const noexcept
{
#if _WIN64
	return 8;
#else
	return 4;
#endif
}

bool IntPtr::IsNull() const noexcept
{
	return m_Ptr == IntPtr::Zero();
}

IntPtr IntPtr::Add(IntPtr p, uint32_t offset) noexcept
{
	return IntPtr(p.m_Ptr + offset);
}

IntPtr& IntPtr::operator=(uint32_t p) noexcept
{
#if _WIN64
	m_Ptr = static_cast<uint64_t>(p);
#else
	m_Ptr = p;
#endif

	return *this;
}

IntPtr& IntPtr::operator=(void* p) noexcept
{
#if _WIN64
	m_Ptr = p == nullptr ? 0 : (uint64_t)(p);
#else
	m_Ptr = p == nullptr ? 0 : (uint32_t)p;
#endif

	return *this;
}

bool IntPtr::operator==(uint32_t p) const noexcept
{
#if _WIN64
	return m_Ptr == static_cast<uint64_t>(p);
#else
	return m_Ptr == p;
#endif
}

bool IntPtr::operator==(void* p) const noexcept
{
#if _WIN64
	return m_Ptr == (p == nullptr ? 0 : (uint64_t)p);
#else
	return m_Ptr == (p == nullptr ? 0 : (uint32_t)p);
#endif
}

#if _WIN64
IntPtr& IntPtr::operator=(uint64_t p) noexcept
{
	m_Ptr = p;
	return *this;
}
bool IntPtr::operator==(uint64_t p) const noexcept
{
	return m_Ptr == p;
}
#endif