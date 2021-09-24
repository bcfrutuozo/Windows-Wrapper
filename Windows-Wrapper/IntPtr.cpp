#include "IntPtr.h"
#include "Exceptions.h"

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

int IntPtr::GetHashCode() const
{
	return static_cast<int>(m_Ptr);
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