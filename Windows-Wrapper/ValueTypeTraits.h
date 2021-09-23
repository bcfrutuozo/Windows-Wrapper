#pragma once

#include <type_traits>
#include <iosfwd>

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

//#pragma once
//
//#include "IEquatable.h"
//#include "IComparable.h"
//#include "IConvertible.h"
//
//#include <iosfwd>
//#include <type_traits>
//
//template<typename TFrom, typename TTo> struct is_promotion : std::false_type {};
//template<typename TFrom, typename TTo> struct is_conversion : std::false_type {};
//template<> struct is_conversion<int, signed char> : std::true_type {};
//template<> struct is_conversion<unsigned int, unsigned char> : std::true_type {};
//template<> struct is_conversion<int, short> : std::true_type {};
//template<> struct is_conversion<unsigned int, unsigned short> : std::true_type {};
//template<> struct is_promotion<signed char, short> : std::true_type {};
//template<> struct is_promotion<unsigned char, unsigned short> : std::true_type {};
//template<> struct is_promotion<signed char, int> : std::true_type {};
//template<> struct is_promotion<short, int> : std::true_type {};
//template<> struct is_promotion<unsigned char, unsigned int> : std::true_type {};
//template<> struct is_promotion<unsigned short, unsigned int> : std::true_type {};
//template<> struct is_promotion<signed char, long> : std::true_type {};
//template<> struct is_promotion<short, long> : std::true_type {};
//template<> struct is_promotion<int, long> : std::true_type {};
//template<> struct is_promotion<unsigned char, unsigned long> : std::true_type {};
//template<> struct is_promotion<unsigned short, unsigned long> : std::true_type {};
//template<> struct is_promotion<unsigned int, unsigned long> : std::true_type {};
//template<> struct is_promotion<signed char, long long> : std::true_type {};
//template<> struct is_promotion<short, long long> : std::true_type {};
//template<> struct is_promotion<int, long long> : std::true_type {};
//template<> struct is_promotion<long, long long> : std::true_type {};
//template<> struct is_promotion<unsigned char, unsigned long long> : std::true_type {};
//template<> struct is_promotion<unsigned short, unsigned long long> : std::true_type {};
//template<> struct is_promotion<unsigned int, unsigned long long> : std::true_type {};
//template<> struct is_promotion<unsigned long, unsigned long long> : std::true_type {};
//template<> struct is_promotion<signed char, double> : std::true_type {};
//template<> struct is_promotion<unsigned char, double> : std::true_type {};
//template<> struct is_promotion<short, double> : std::true_type {};
//template<> struct is_promotion<unsigned short, double> : std::true_type {};
//template<> struct is_promotion<int, double> : std::true_type {};
//template<> struct is_promotion<unsigned int, double> : std::true_type {};
//template<> struct is_promotion<long, double> : std::true_type {};
//template<> struct is_promotion<unsigned long, double> : std::true_type {};
//template<> struct is_promotion<float, double> : std::true_type {};
//template<> struct is_promotion<signed char, long double> : std::true_type {};
//template<> struct is_promotion<unsigned char, long double> : std::true_type {};
//template<> struct is_promotion<short, long double> : std::true_type {};
//template<> struct is_promotion<unsigned short, long double> : std::true_type {};
//template<> struct is_promotion<int, long double> : std::true_type {};
//template<> struct is_promotion<unsigned int, long double> : std::true_type {};
//template<> struct is_promotion<long, long double> : std::true_type {};
//template<> struct is_promotion<unsigned long, long double> : std::true_type {};
//template<> struct is_promotion<long long, long double> : std::true_type {};
//template<> struct is_promotion<unsigned long long, long double> : std::true_type {};
//template<> struct is_promotion<float, long double> : std::true_type {};
//template<> struct is_promotion<double, long double> : std::true_type {};
//
//template<typename T, typename = std::enable_if_t< std::is_arithmetic<T>::value>>
//class ValueType : public IConvertible, public IEquatable<T>, public IComparable<T>
//{
//protected:
//
//	T m_value;
//
//public:
//
//	using value_type = T;
//
//	constexpr ValueType() noexcept
//		:
//		m_value()
//	{}
//
//	template<typename U, typename = std::enable_if_t<std::is_same<T, U>::value || is_promotion<U, T>::value>>
//	constexpr ValueType(U const& value) noexcept
//		:
//		m_value(value)
//	{}
//
//	template<typename U, typename = std::enable_if_t< is_promotion<U, T>::value>>
//	constexpr ValueType(ValueType<U> const& other) noexcept
//		:
//		m_value(other.get())
//	{}
//
//	template<typename U, typename = std::enable_if_t< is_conversion<U, T>::value>>
//	constexpr static ValueType from(U const& other) noexcept
//	{
//		return ValueType(T(other));
//	}
//
//	ValueType(ValueType const&) = default;
//	ValueType(ValueType&&) = default;
//	ValueType& operator=(ValueType const&) = default;
//	ValueType& operator=(ValueType&&) = default;
//
//	ValueType(value_type const& other)
//		:
//		m_value(other)
//	{ }
//
//	ValueType& operator=(value_type const& other)
//	{
//		m_value = m_value;
//		return *this;
//	}
//
//	constexpr T const& get() const noexcept { return m_value; }
//
//	template<typename U = T, typename = std::enable_if_t< !std::is_same<U, bool>::value>>
//	constexpr ValueType const& operator+() const noexcept
//	{
//		return *this;
//	}
//	template<typename U = T, typename = std::enable_if_t< !std::is_same<U, bool>::value>>
//	constexpr ValueType operator-() const noexcept
//	{
//		return ValueType(-m_value);
//	}
//
//	template<typename U = T, typename = std::enable_if_t< std::is_integral<U>::value && !std::is_same<U, bool>::value>>
//	constexpr ValueType operator~() const noexcept
//	{
//		return ValueType(~m_value);
//	}
//
//	template<typename U = T, typename = std::enable_if_t< std::is_same<U, bool>::value >>
//	constexpr bool operator!() const noexcept
//	{
//		return !m_value;
//	}
//
//	ValueType& operator++() noexcept
//	{
//		++m_value;
//		return *this;
//	}
//	ValueType operator++(int) noexcept
//	{
//		return ValueType(m_value++);
//	}
//
//	ValueType& operator--() noexcept
//	{
//		--m_value;
//		return *this;
//	}
//	ValueType operator--(int) noexcept
//	{
//		return ValueType(m_value--);
//	}
//
//	template<typename U>
//	ValueType& operator+=(U const& other) noexcept
//	{
//		m_value += other;
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator+=(ValueType<U> const& other) noexcept
//	{
//		m_value += other.get();
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator+=(value_type const& other) noexcept
//	{
//		m_value += other;
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator-=(U const& other) noexcept
//	{
//		m_value -= other;
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator-=(ValueType<U> const& other) noexcept
//	{
//		m_value -= other.get();
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator*=(U const& other) noexcept
//	{
//		m_value *= other;
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator*=(ValueType<U> const& other) noexcept
//	{
//		m_value *= other.get();
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator/=(U const& other) noexcept
//	{
//		m_value /= other;
//		return *this;
//	}
//
//	template<typename U>
//	ValueType& operator/=(ValueType<U> const& other) noexcept
//	{
//		m_value /= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator%=(U const& other) noexcept
//	{
//		m_value %= other;
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator%=(ValueType<U> const& other) noexcept
//	{
//		m_value %= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator<<=(U const& other) noexcept
//	{
//		m_value <<= other;
//		return *this;
//	}
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator<<=(ValueType<U> const& other) noexcept
//	{
//		m_value <<= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator>>=(U const& other) noexcept
//	{
//		m_value >>= other;
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::valu >>
//	ValueType& operator>>=(ValueType<U> const& other) noexcept
//	{
//		m_value >>= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator&=(U const& other) noexcept
//	{
//		m_value &= other;
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator&=(ValueType<U> const& other) noexcept
//	{
//		m_value &= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator|=(U const& other) noexcept
//	{
//		m_value |= other;
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator|=(ValueType<U> const& other) noexcept
//	{
//		m_value |= other.get();
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator^=(U const& other) noexcept
//	{
//		m_value ^= other;
//		return *this;
//	}
//
//	template<typename U, typename = std::enable_if_t< std::is_integral<T>::value&& std::is_integral<U>::value>>
//	ValueType& operator^=(ValueType<U> const& other) noexcept
//	{
//		m_value ^= other.get();
//		return *this;
//	}
//
//	template<typename U>
//	constexpr explicit operator ValueType<U>() const noexcept
//	{
//		return ValueType<U>(static_cast<U>(m_value));
//	}
//
//	friend std::istream& operator>>(std::istream& lhs, ValueType<T>& rhs)
//	{
//		return lhs >> rhs.m_value;
//	}
//
//	int CompareTo(const Object* const b) const override;
//	int CompareTo(const T* const b) const override;
//	bool Equals(const Object* const obj) const noexcept override;
//	bool Equals(const T* const value) const noexcept override;
//	ValueType<int32_t> ToInt32(IFormatProvider* provider) const override;
//};
//
//template<typename T>
//constexpr ValueType<T> operator+(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() + rhs);
//}
//
//template<typename T>
//constexpr ValueType<T> operator+(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs + rhs.get());
//}
//
//template<typename T1, typename T2>
//constexpr auto operator+(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() + rhs.get())>(lhs.get() + rhs.get());
//}
//
//template<typename T>
//constexpr ValueType<T> operator-(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() - rhs);
//}
//
//template<typename T>
//constexpr ValueType<T> operator-(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs - rhs.get());
//}
//
//template<typename T1, typename T2>
//constexpr auto operator-(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() - rhs.get())>(lhs.get() - rhs.get());
//}
//
//template<typename T>
//constexpr ValueType<T> operator*(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() * rhs);
//}
//
//template<typename T>
//constexpr ValueType<T> operator*(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs * rhs.get());
//}
//
//template<typename T1, typename T2>
//constexpr auto operator*(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get()* rhs.get())>(lhs.get() * rhs.get());
//}
//
//template<typename T>
//constexpr ValueType<T> operator/(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() / rhs);
//}
//
//template<typename T>
//constexpr ValueType<T> operator/(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs / rhs.get());
//}
//
//template<typename T1, typename T2>
//constexpr auto operator/(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() / rhs.get())>(lhs.get() / rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator%(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() % rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator%(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs % rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator%(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() % rhs.get())>(lhs.get() % rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator&(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() & rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator&(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs & rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator&(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get()& rhs.get())>(lhs.get() & rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator|(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() | rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator|(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs | rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator|(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() | rhs.get())>(lhs.get() | rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator^(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() ^ rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator^(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs ^ rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator^(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() ^ rhs.get())>(lhs.get() ^ rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator<<(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() << rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator<<(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs << rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator<<(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() << rhs.get())>(lhs.get() << rhs.get());
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator>>(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return ValueType<T>(lhs.get() >> rhs);
//}
//
//template<typename T, typename = std::enable_if_t< std::is_integral<T>::value>>
//constexpr ValueType<T> operator>>(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return ValueType<T>(lhs >> rhs.get());
//}
//
//template<typename T1, typename T2, typename = std::enable_if_t< std::is_integral<T1>::value&& std::is_integral<T2>::value>>
//constexpr auto operator>>(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return ValueType<decltype(lhs.get() >> rhs.get())>(lhs.get() >> rhs.get());
//}
//
//constexpr bool operator&&(ValueType<bool> const& lhs, bool const& rhs) noexcept
//{
//	return lhs.get() && rhs;
//}
//
//constexpr bool operator&&(bool const& lhs, ValueType<bool> const& rhs) noexcept
//{
//	return lhs && rhs.get();
//}
//
//constexpr bool operator&&(ValueType<bool> const& lhs, ValueType<bool> const& rhs) noexcept
//{
//	return lhs.get() && rhs.get();
//}
//
//constexpr bool operator||(ValueType<bool> const& lhs, bool const& rhs) noexcept
//{
//	return lhs.get() || rhs;
//}
//
//constexpr bool operator||(bool const& lhs, ValueType<bool> const& rhs) noexcept
//{
//	return lhs || rhs.get();
//}
//
//constexpr bool operator||(ValueType<bool> const& lhs, ValueType<bool> const& rhs) noexcept
//{
//	return lhs.get() || rhs.get();
//}
//
//template<typename T>
//constexpr bool operator==(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() == rhs;
//}
//
//template<typename T>
//constexpr bool operator==(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs == rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator==(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() == rhs.get();
//}
//
//template<typename T>
//constexpr bool operator!=(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() != rhs;
//}
//
//template<typename T>
//constexpr bool operator!=(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs != rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator!=(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() != rhs.get();
//}
//
//template<typename T>
//constexpr bool operator<(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() < rhs;
//}
//
//template<typename T>
//constexpr bool operator<(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs < rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator<(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() < rhs.get();
//}
//
//template<typename T>
//constexpr bool operator<=(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() <= rhs;
//}
//
//template<typename T>
//constexpr bool operator<=(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs <= rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator<=(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() <= rhs.get();
//}
//
//template<typename T>
//constexpr bool operator>(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() > rhs;
//}
//
//template<typename T>
//constexpr bool operator>(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs > rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator>(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() > rhs.get();
//}
//
//template<typename T>
//constexpr bool operator>=(ValueType<T> const& lhs, T const& rhs) noexcept
//{
//	return lhs.get() >= rhs;
//}
//
//template<typename T>
//constexpr bool operator>=(T const& lhs, ValueType<T> const& rhs) noexcept
//{
//	return lhs >= rhs.get();
//}
//
//template<typename T1, typename T2>
//constexpr bool operator>=(ValueType<T1> const& lhs, ValueType<T2> const& rhs) noexcept
//{
//	return lhs.get() >= rhs.get();
//}
//
//template<typename T>
//std::ostream& operator<<(std::ostream& lhs, ValueType<T> const& rhs)
//{
//	return lhs << rhs.get();
//}
//
//using Int32 = ValueType<int32_t>;
//
////template<>
////class ValueType<int32_t>
////{
////public:
////
////	int CompareTo(const Object* const obj) const
////	{
////		if (obj == nullptr) return false;
////
////		if (const auto value = dynamic_cast<const ValueType<int32_t>*>(obj))
////		{
////			return *this == *value;
////		}
////
////		if (const auto value = (int32_t*)(obj))
////		{
////			return m_value == *value;
////		}
////
////		throw ArgumentException("Arg_MustBeInt32");
////	}
////
////	int CompareTo(const int32_t* const value) const
////	{
////		if (value == nullptr) return false;
////		return m_value == *value;
////	}
////
////	bool Equals(const Object* const obj) const noexcept
////	{
////		return true;
////	}
////
////	bool Equals(const int32_t* const b) const noexcept
////	{
////		return true;
////	}
////
////	ValueType<int32_t> ToInt32(IFormatProvider* provider) const
////	{
////		return ValueType<int32_t>();
////	}
////};