#pragma once

//#include "Boolean.h"
//#include "Byte.h"
//#include "Int32.h"

#include <type_traits>

template<typename TFrom, typename TTo> struct is_promotion : std::false_type {};
template<typename TFrom, typename TTo> struct is_conversion : std::false_type {};

template<> struct is_conversion<int, signed char> : std::true_type {};
template<> struct is_conversion<unsigned int, unsigned char> : std::true_type {};
template<> struct is_conversion<int, short> : std::true_type {};
template<> struct is_conversion<unsigned int, unsigned short> : std::true_type {};

template<> struct is_promotion<signed char, short> : std::true_type {};

template<> struct is_promotion<unsigned char, unsigned short> : std::true_type {};

template<> struct is_promotion<signed char, int> : std::true_type {};
template<> struct is_promotion<short, int> : std::true_type {};

template<> struct is_promotion<unsigned char, unsigned int> : std::true_type {};
template<> struct is_promotion<unsigned short, unsigned int> : std::true_type {};

template<> struct is_promotion<signed char, long> : std::true_type {};
template<> struct is_promotion<short, long> : std::true_type {};
template<> struct is_promotion<int, long> : std::true_type {};

template<> struct is_promotion<unsigned char, unsigned long> : std::true_type {};
template<> struct is_promotion<unsigned short, unsigned long> : std::true_type {};
template<> struct is_promotion<unsigned int, unsigned long> : std::true_type {};

template<> struct is_promotion<signed char, long long> : std::true_type {};
template<> struct is_promotion<short, long long> : std::true_type {};
template<> struct is_promotion<int, long long> : std::true_type {};
template<> struct is_promotion<long, long long> : std::true_type {};

template<> struct is_promotion<unsigned char, unsigned long long> : std::true_type {};
template<> struct is_promotion<unsigned short, unsigned long long> : std::true_type {};
template<> struct is_promotion<unsigned int, unsigned long long> : std::true_type {};
template<> struct is_promotion<unsigned long, unsigned long long> : std::true_type {};

template<> struct is_promotion<signed char, double> : std::true_type {};
template<> struct is_promotion<unsigned char, double> : std::true_type {};
template<> struct is_promotion<short, double> : std::true_type {};
template<> struct is_promotion<unsigned short, double> : std::true_type {};
template<> struct is_promotion<int, double> : std::true_type {};
template<> struct is_promotion<unsigned int, double> : std::true_type {};
template<> struct is_promotion<long, double> : std::true_type {};
template<> struct is_promotion<unsigned long, double> : std::true_type {};
template<> struct is_promotion<float, double> : std::true_type {};

template<> struct is_promotion<signed char, long double> : std::true_type {};
template<> struct is_promotion<unsigned char, long double> : std::true_type {};
template<> struct is_promotion<short, long double> : std::true_type {};
template<> struct is_promotion<unsigned short, long double> : std::true_type {};
template<> struct is_promotion<int, long double> : std::true_type {};
template<> struct is_promotion<unsigned int, long double> : std::true_type {};
template<> struct is_promotion<long, long double> : std::true_type {};
template<> struct is_promotion<unsigned long, long double> : std::true_type {};
template<> struct is_promotion<long long, long double> : std::true_type {};
template<> struct is_promotion<unsigned long long, long double> : std::true_type {};
template<> struct is_promotion<float, long double> : std::true_type {};
template<> struct is_promotion<double, long double> : std::true_type {};
//#include "StringWrapper.h"
//#include "Char.h"


// EXCLUSIVE bool operations
//constexpr bool operator&&(Boolean const& lhs, bool const& rhs) noexcept
//{
//	return lhs.Get() && rhs;
//}
////
////constexpr bool operator&&(bool const& lhs, Boolean const& rhs) noexcept
//{
//	return lhs && rhs.Get();
//}
////
////constexpr bool operator&&(Boolean const& lhs, Boolean const& rhs) noexcept
//{
//	return lhs.Get() && rhs.Get();
//}
////
////constexpr bool operator||(Boolean const& lhs, bool const& rhs) noexcept
//{
//	return lhs.Get() || rhs;
//}
////
////constexpr bool operator||(bool const& lhs, Boolean const& rhs) noexcept
//{
//	return lhs || rhs.Get();
//}
////
////constexpr bool operator||(Boolean const& lhs, Boolean const& rhs) noexcept
//{
//	return lhs.Get() || rhs.Get();
//}
//
//// Boolean operations
//constexpr Boolean operator+(Boolean& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs.Get() + rhs.Get());
//}
//
////constexpr Boolean operator+(Boolean& const lhs, Char& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, SByte& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, Byte& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, Int16& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, UInt16& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
//
//constexpr Boolean operator+(Boolean& const lhs, Int32& const rhs)
//{
//	return Boolean(lhs.Get() + rhs.Get());
//}
//
////constexpr Boolean operator+(Boolean& const lhs, UInt32& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, Int64& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, UInt64& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, Single& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
////
////constexpr Boolean operator+(Boolean& const lhs, Double& const rhs)
////{
////	return Boolean(lhs.Get() + rhs.Get());
////}
//
//
//constexpr Boolean operator+(Boolean& const lhs, bool& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, char& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, signed char& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, unsigned char& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, int16_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, uint16_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, int32_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, uint32_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, int64_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, uint64_t& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, float& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(Boolean& const lhs, double& const rhs)
//{
//	return Boolean(lhs + rhs);
//}
//
//constexpr Boolean operator+(bool& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(char& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(signed char& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(unsigned char& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(int16_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(uint16_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(int32_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(uint32_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(int64_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(uint64_t& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(float& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator+(double& const lhs, Boolean& const rhs)
//{
//	return Boolean(lhs + rhs.Get());
//}
//
//constexpr Boolean operator==(Boolean& const lhs, Boolean& const rhs)
//{
//	return lhs == rhs;
//}
//
//constexpr Boolean operator==(Boolean& const lhs, bool& const rhs)
//{
//	return lhs.Get() == rhs;
//}
//
//constexpr Boolean operator==(bool& const lhs, Boolean& const rhs)
//{
//	return lhs == rhs.Get();
//}
//
//constexpr Boolean operator!=(Boolean& const lhs, Boolean& const rhs)
//{
//	return lhs != rhs;
//}
//
//constexpr Boolean operator!=(Boolean& const lhs, bool& const rhs)
//{
//	return lhs.Get() != rhs;
//}
//
//constexpr Boolean operator!=(bool& const lhs, Boolean& const rhs)
//{
//	return lhs != rhs.Get();
//}