#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Int64 final : public Primitive<long long>, public IComparable<Int64>, public IConvertible, public IEquatable<Int64>
{
public:

	constexpr Int64() noexcept : Primitive() {}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64(const U& value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr Int64(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Int64(const Int64& value) noexcept : Primitive(value) { }

	Int64(Int64&&) = default;
	Int64& operator=(Int64 const&) = default;
	Int64& operator=(Int64&&) = default;

	constexpr Int64 const& operator+() const noexcept { return *this; }
	constexpr Int64 operator-() const noexcept { return Int64(-m_value); }
	constexpr Int64 operator~() const noexcept { return Int64(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr Int64& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr Int64 operator++(int) noexcept
	{
		return Int64(m_value++);
	}

	constexpr Int64& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr Int64 operator--(int) noexcept
	{
		return Int64(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Int64& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int64& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int64& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int64& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long long>::value&& std::is_integral<U>::value>>
	constexpr Int64& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator+(U const& other) noexcept { return Int64(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator-(U const& other) noexcept { return Int64(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator*(U const& other) noexcept { return Int64(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator/(U const& other) noexcept { return Int64(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator%(U const& other) noexcept { return Int64(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator&(U const& other) noexcept { return Int64(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator|(U const& other) noexcept { return Int64(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator^(U const& other) noexcept { return Int64(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator<<(U const& other) noexcept { return Int64(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int64 operator>>(U const& other) noexcept { return Int64(m_value >> other); }

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
	constexpr Int64 operator+(Primitive<U> const& other) const noexcept { return Int64(m_value + other.Get()); }

	template<typename U>
	constexpr Int64 operator-(Primitive<U> const& other) const noexcept { return Int64(m_value - other.Get()); }

	template<typename U>
	constexpr Int64 operator*(Primitive<U> const& other) const noexcept { return Int64(m_value * other.Get()); }

	template<typename U>
	constexpr Int64 operator/(Primitive<U> const& other) const noexcept { return Int64(m_value / other.Get()); }

	template<typename U>
	constexpr Int64 operator%(Primitive<U> const& other) const noexcept { return Int64(m_value % other.Get()); }

	template<typename U>
	constexpr Int64 operator&(Primitive<U> const& other) const noexcept { return Int64(m_value & other.Get()); }

	template<typename U>
	constexpr Int64 operator|(Primitive<U> const& other) const noexcept { return Int64(m_value | other.Get()); }

	template<typename U>
	constexpr Int64 operator^(Primitive<U> const& other) const noexcept { return Int64(m_value ^ other.Get()); }

	template<typename U>
	constexpr Int64 operator<<(Primitive<U> const& other) const noexcept { return Int64(m_value << other.Get()); }

	template<typename U>
	constexpr Int64 operator>>(Primitive<U> const& other) const noexcept { return Int64(m_value >> other.Get()); }

	template<typename U>
	constexpr bool operator==(Primitive<U> const& other) const noexcept { return m_value == other.Get(); }

	template<typename U>
	constexpr bool operator!=(Primitive<U> const& other) const noexcept { return m_value != other.Get(); }

	template<typename U>
	constexpr bool operator<(Primitive<U> const& other) const noexcept { return m_value < other.Get(); }

	template<typename U>
	constexpr bool operator<=(Primitive<U> const& other) const noexcept { return m_value <= other.Get(); }

	template<typename U>
	constexpr bool operator>(Primitive<U> const& other) const noexcept { return m_value > other.Get(); }

	template<typename U>
	constexpr bool operator>=(Primitive<U> const& other) const noexcept { return m_value >= other.Get(); }

	// Output/input stream operator
	friend std::istream& operator>>(std::istream& lhs, Int64& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const Int64& rhs) { return lhs << rhs.Get(); }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Int64; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Int64* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Int64* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Char ToChar(IFormatProvider* provider) const override;
	SByte ToSByte(IFormatProvider* provider) const override;
	Byte ToByte(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	UInt16 ToUInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;
	UInt32 ToUInt32(IFormatProvider* provider) const override;
	Int64 ToInt64(IFormatProvider* provider) const override;
	UInt64 ToUInt64(IFormatProvider* provider) const override;
	Single ToSingle(IFormatProvider* provider) const override;
	Double ToDouble(IFormatProvider* provider) const override;

	// Exposed class expressions
	static constexpr Int64 MaxValue() { return Int64(0x7FFFFFFFFFFFFFFFL); }
	static constexpr Int64 MinValue() { return Int64(static_cast<long long>(0x8000000000000000L)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator+(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator-(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator*(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator/(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator%(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator&(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator|(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator^(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator<<(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int64 operator>>(T const& lhs, Int64 const& rhs) noexcept
{
	return Int64(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, Int64 const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, std::is_fundamental<T>::value>
constexpr bool operator!=(T const& lhs, Int64 const& rhs) noexcept
{
	return lhs != rhs.Get();
}