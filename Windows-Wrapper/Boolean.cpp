#include "Boolean.h"
#include "Exceptions.h"

inline int Boolean::GetHashCode() const noexcept
{
	return m_value ? 1 : 0;
}

int Boolean::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Boolean*>(obj))
	{
		int i = value->Get();

		if (m_value == i) return 0;
		else if (m_value == false) return - 1;
		else return 1;
	}

	if (const auto value = (bool*)(obj))
	{
		if (m_value == *value) return 0;
		else if (m_value == false) return -1;
		else return 1;
	}

	throw ArgumentException("Arg_MustBeBoolean");
}

int Boolean::CompareTo(const Boolean* const value) const
{
	if (value == nullptr) return 1;

	if (m_value == *value) return 0;
	else if (m_value == false) return -1;
	else return 1;
}

bool Boolean::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Int32*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (int32_t*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Boolean::Equals(const Boolean* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Boolean::ToBoolean(IFormatProvider* provider) const
{
	return *this;
}

Int32 Boolean::ToInt32(IFormatProvider* provider) const
{
	// CONVERT BOOL TO INT32
	return 0;
}