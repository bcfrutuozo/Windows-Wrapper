#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"
#include "Int32.h"

class Boolean final : public Primitive<bool>, public IComparable<Boolean>, public IConvertible, public IEquatable<Boolean>
{
private:

	constexpr Boolean(Primitive<bool> const& value) : Primitive(value.Get()) { }

public:

	constexpr Boolean() noexcept : Primitive() {}
	constexpr Boolean(const bool& value) noexcept : Primitive(value) { }
	constexpr Boolean(const Boolean& value) noexcept : Primitive(value.Get()) {	}

	Boolean(Boolean&&) = default;
	Boolean& operator=(Boolean const&) = default;
	Boolean& operator=(Boolean&&) = default;

	template<typename U, typename = std::enable_if_t< is_conversion<U, bool>::value >>
	constexpr static Boolean From(U const& other) noexcept { return Boolean(U(other)); }

	constexpr Boolean const& operator+() const noexcept { return *this; }
	constexpr bool operator!() const noexcept { return !m_value; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<bool>::value&& std::is_integral<U>::value >>
	constexpr Boolean& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Boolean exclusive
	constexpr bool operator&&(Boolean const& other) noexcept { return m_value && other.Get(); }
	constexpr bool operator||(Boolean const& other) noexcept { return m_value || other.Get(); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator+(U const& other) noexcept { return Boolean(m_value + other); }

	template<typename U>
	constexpr Boolean operator+(Primitive<U> const& other) const noexcept { return Boolean(other.Get() + m_value); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator-(U const& other) noexcept { return Boolean(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator*(U const& other) noexcept { return Boolean(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator/(U const& other) noexcept { return Boolean(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator%(U const& other) noexcept { return Boolean(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator&(U const& other) noexcept { return Boolean(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator|(U const& other) noexcept { return Boolean(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator^(U const& other) noexcept { return Boolean(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator<<(U const& other) noexcept { return Boolean(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator>>(U const& other) noexcept { return Boolean(m_value >> other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator==(U const& other) noexcept { return m_value == other; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator!=(U const& other) noexcept { return m_value != other; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator<(U const& other) noexcept { return m_value < other; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator<=(U const& other) noexcept { return m_value <= other; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator>(U const& other) noexcept { return m_value > other; }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr bool operator>=(U const& other) noexcept { return m_value >= other; }

	friend std::istream& operator>>(std::istream& lhs, Boolean& const rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, Boolean& const rhs) { return lhs << rhs.m_value; }

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Boolean; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Boolean* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Boolean* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	static constexpr Int32 True() { return 1; }
	static constexpr Int32 False() { return 0; }
};

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator+(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator-(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator*(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator/(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator%(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator&(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator|(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator^(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator<<(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Boolean operator>>(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator!=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs != rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator<(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator<=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator>(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs > rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator>=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs >= rhs.Get();
}