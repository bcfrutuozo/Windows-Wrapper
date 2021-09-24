//#include "Byte.h"
//#include "Boolean.h"
//#include "Exceptions.h"
//
//Boolean Byte::operator!() const noexcept
//{
//	return Boolean(!m_value);
//}
//
//inline int Byte::GetHashCode() const
//{
//	return static_cast<int>(m_value);
//}
//
//int Byte::CompareTo(const Object* const obj) const
//{
//	if (obj == nullptr) return 1;
//
//	if (const auto value = dynamic_cast<const Byte*>(obj))
//	{
//		int i = value->Get();
//
//		if (m_value < i) return -1;
//		if (m_value > i) return 1;
//
//		return 0;
//	}
//
//	if (const auto value = (byte*)(obj))
//	{
//		if (m_value < *value) return -1;
//		if (m_value > *value) return 1;
//
//		return 0;
//	}
//
//	throw ArgumentException("Arg_MustBeInt32");
//}
//
//int Byte::CompareTo(const Byte* const value) const
//{
//	if (value == nullptr) return 1;
//
//	if (m_value < value->Get()) return -1;
//	if (m_value > value->Get()) return 1;
//
//	return 0;
//}
//
//bool Byte::Equals(const Object* const obj) const
//{
//	if (obj == nullptr) return false;
//
//	if (const auto value = dynamic_cast<const Byte*>(obj))
//	{
//		return Get() == value->Get();
//	}
//
//	if (const auto value = (byte*)(obj))
//	{
//		return Get() == *value;
//	}
//
//	return false;
//}
//
//bool Byte::Equals(const Byte* const b) const
//{
//	if (b == nullptr) return false;
//
//	return Get() == b->Get();
//}
//
//Boolean Byte::ToBoolean(IFormatProvider* provider) const
//{
//	//TODO:: CONVERT INT32 TO BOOLEAN
//	return false;
//}
//
//Int32 Byte::ToInt32(IFormatProvider* provider) const
//{
//	return 0;
//}