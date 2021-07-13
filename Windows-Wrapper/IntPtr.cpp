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

void* IntPtr::ToPointer() const noexcept
{
	return m_Ptr;
}

bool IntPtr::Equals(IntPtr p) const noexcept
{
	return m_Ptr == p.m_Ptr;
}

int IntPtr::ToInt32() const noexcept
{
	return reinterpret_cast<int>(m_Ptr);
}

long IntPtr::ToInt64() const noexcept
{
	return reinterpret_cast<long>(m_Ptr);
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