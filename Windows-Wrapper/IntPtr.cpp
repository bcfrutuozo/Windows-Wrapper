#include "IntPtr.h"

IntPtr::IntPtr(int p)
	:
	m_Ptr(reinterpret_cast<void*>(p))
{

}

IntPtr::IntPtr(long p)
	:
	m_Ptr(reinterpret_cast<void*>(p))
{

}

IntPtr::IntPtr(void* p)
	:
	m_Ptr(p)
{

}

IntPtr::~IntPtr()
{

}

bool IntPtr::operator==(const IntPtr& p) const noexcept
{
	return m_Ptr == p.m_Ptr;
}

bool IntPtr::Equals(const IntPtr& p) const noexcept
{
	return m_Ptr == p.m_Ptr;
}

// Remove compile warnings regarding pointer conversions
#pragma warning(push)
#pragma warning(disable : 4244)

int IntPtr::ToInt32() const noexcept
{
	return reinterpret_cast<intptr_t>(m_Ptr);
}

long IntPtr::ToInt64() const noexcept
{
	return reinterpret_cast<intptr_t>(m_Ptr);
}

#pragma warning(pop)

void* IntPtr::ToPointer() const noexcept
{
	return m_Ptr;
}

int IntPtr::Size() const noexcept
{
#if _WIN32
	return 4;
#else
	return 8;
#endif
}

bool IntPtr::IsNull() const noexcept
{
	return m_Ptr == nullptr;
}

IntPtr IntPtr::Add(IntPtr p, int offset) noexcept
{
	return IntPtr(static_cast<long>(reinterpret_cast<intptr_t>(p.m_Ptr) + offset));
}

IntPtr& IntPtr::operator=(int p) noexcept
{
	m_Ptr = reinterpret_cast<void*>(p);
	return *this;
}

IntPtr& IntPtr::operator=(long p) noexcept
{
	m_Ptr = reinterpret_cast<void*>(p);
	return *this;
}

IntPtr& IntPtr::operator=(void* p) noexcept
{
	m_Ptr = p;
	return *this;
}

bool IntPtr::operator==(int p) const noexcept
{
	return m_Ptr == reinterpret_cast<void*>(p);
}

bool IntPtr::operator==(long p) const noexcept
{
	return m_Ptr == reinterpret_cast<void*>(p);
}

bool IntPtr::operator==(void* p) const noexcept
{
	return m_Ptr == p;
}