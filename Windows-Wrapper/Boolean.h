#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"
#include "Int32.h"

class Boolean final : public Primitive<bool>, public IComparable<Boolean>, public IConvertible, public IEquatable<Boolean>
{
public:

	constexpr Boolean() noexcept : Primitive() {}
	
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean(const U& value) noexcept : Primitive(value) {}
	
	template<typename U>
	constexpr Boolean(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Boolean(const Boolean& value) noexcept : Primitive(value) { }

	Boolean(Boolean&&) = default;
	Boolean& operator=(Boolean const&) = default;
	Boolean& operator=(Boolean&&) = default;

	constexpr Boolean const& operator+() const noexcept { return *this; }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator+=(U const& other) noexcept {
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Boolean& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.m_value;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.m_value;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.m_value;
		return *this;
	}

	template<typename U>
	constexpr Boolean& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.m_value;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<bool>::value&& std::is_integral<U>::value>>
	constexpr Boolean& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.m_value;
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Boolean operator+(U const& other) noexcept { return Boolean(m_value + other); }

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

	// Operators with Primitives<U> type
	template<typename U>
	constexpr Boolean operator+(Primitive<U> const& other) const noexcept { return Boolean(m_value + other.m_value); }

	template<typename U>
	constexpr Boolean operator-(Primitive<U> const& other) const noexcept { return Boolean(m_value - other.m_value); }

	template<typename U>
	constexpr Boolean operator*(Primitive<U> const& other) const noexcept { return Boolean(m_value * other.m_value); }

	template<typename U>
	constexpr Boolean operator/(Primitive<U> const& other) const noexcept { return Boolean(m_value / other.m_value); }

	template<typename U>
	constexpr Boolean operator%(Primitive<U> const& other) const noexcept { return Boolean(m_value % other.m_value); }

	template<typename U>
	constexpr Boolean operator&(Primitive<U> const& other) const noexcept { return Boolean(m_value & other.m_value); }

	template<typename U>
	constexpr Boolean operator|(Primitive<U> const& other) const noexcept { return Boolean(m_value | other.m_value); }

	template<typename U>
	constexpr Boolean operator^(Primitive<U> const& other) const noexcept { return Boolean(m_value ^ other.m_value); }

	template<typename U>
	constexpr Boolean operator<<(Primitive<U> const& other) const noexcept { return Boolean(m_value << other.m_value); }

	template<typename U>
	constexpr Boolean operator>>(Primitive<U> const& other) const noexcept { return Boolean(m_value >> other.m_value); }

	template<typename U>
	constexpr Boolean operator==(Primitive<U> const& other) const noexcept { return Boolean(m_value == other.m_value); }

	template<typename U>
	constexpr Boolean operator!=(Primitive<U> const& other) const noexcept { return Boolean(m_value != other.m_value); }

	template<typename U>
	constexpr Boolean operator<(Primitive<U> const& other) const noexcept { return Boolean(m_value < other.m_value); }

	template<typename U>
	constexpr Boolean operator<=(Primitive<U> const& other) const noexcept { return Boolean(m_value <= other.m_value); }

	template<typename U>
	constexpr Boolean operator>(Primitive<U> const& other) const noexcept { return Boolean(m_value > other.m_value); }

	template<typename U>
	constexpr Boolean operator>=(Primitive<U> const& other) const noexcept { return Boolean(m_value >= other.m_value); }

	// Output/input stream operator
	friend std::istream& operator>>(std::istream& lhs, Boolean& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const Boolean&rhs) { return lhs << rhs.m_value; }

	// Logical AND/OR operators (Boolean exclusive)
	constexpr bool operator&&(bool const& other) noexcept { return m_value && other; }
	constexpr bool operator||(bool const& other) noexcept { return m_value || other; }
	constexpr bool operator&&(Boolean const& other) noexcept { return m_value && other.Get(); }
	constexpr bool operator||(Boolean const& other) noexcept { return m_value || other.Get(); }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Boolean; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Boolean* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Boolean* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Char ToChar(IFormatProvider* provider) const override;
	Byte ToByte(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	UInt16 ToUInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	// Exposed expressions
	static constexpr Int32 True() { return 1; }
	static constexpr Int32 False() { return 0; }
};

// Operators for operations with fundamental types as first parameters
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
	return Boolean(lhs * rhs.Get());
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

template<typename T>
constexpr bool operator<(T const& lhs, Primitive<T> const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T>
constexpr bool operator<=(T const& lhs, Primitive<T> const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T>
constexpr bool operator>(T const& lhs, Primitive<T> const& rhs) noexcept
{
	return lhs > rhs.Get();
}

template<typename T>
constexpr bool operator>=(T const& lhs, Primitive<T> const& rhs) noexcept
{
	return lhs >= rhs.Get();
}