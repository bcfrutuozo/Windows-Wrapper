#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Byte final: public Primitive<unsigned char>, public IComparable<Byte>, public IConvertible, public IEquatable<Byte>
{
public:

	constexpr Byte() noexcept : Primitive() {}
	
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte(const U& const value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr Byte(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Byte(const Byte& value) noexcept : Primitive(value) { }

	Byte(Byte&&) = default;
	Byte& operator=(Byte const&) = default;
	Byte& operator=(Byte&&) = default;

	constexpr Byte const& operator+() const noexcept { return *this; }
	// operator-() cannot be used with unsigned int because it can lead to undefined behavior
	constexpr Byte operator~() const noexcept { return Byte(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr Byte& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr Byte operator++(int) noexcept
	{
		return Byte(m_value++);
	}

	constexpr Byte& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr Byte operator--(int) noexcept
	{
		return Byte(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Byte& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Byte& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Byte& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Byte& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned char>::value&& std::is_integral<U>::value>>
	constexpr Byte& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator+(U const& other) noexcept { return Byte(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator-(U const& other) noexcept { return Byte(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator*(U const& other) noexcept { return Byte(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator/(U const& other) noexcept { return Byte(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator%(U const& other) noexcept { return Byte(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator&(U const& other) noexcept { return Byte(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator|(U const& other) noexcept { return Byte(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator^(U const& other) noexcept { return Byte(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator<<(U const& other) noexcept { return Byte(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Byte operator>>(U const& other) noexcept { return Byte(m_value >> other); }

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
	constexpr Byte operator+(Primitive<U> const& other) const noexcept { return Byte(m_value + other.Get()); }

	template<typename U>
	constexpr Byte operator-(Primitive<U> const& other) const noexcept { return Byte(m_value - other.Get()); }

	template<typename U>
	constexpr Byte operator*(Primitive<U> const& other) const noexcept { return Byte(m_value * other.Get()); }

	template<typename U>
	constexpr Byte operator/(Primitive<U> const& other) const noexcept { return Byte(m_value / other.Get()); }

	template<typename U>
	constexpr Byte operator%(Primitive<U> const& other) const noexcept { return Byte(m_value % other.Get()); }

	template<typename U>
	constexpr Byte operator&(Primitive<U> const& other) const noexcept { return Byte(m_value & other.Get()); }

	template<typename U>
	constexpr Byte operator|(Primitive<U> const& other) const noexcept { return Byte(m_value | other.Get()); }

	template<typename U>
	constexpr Byte operator^(Primitive<U> const& other) const noexcept { return Byte(m_value ^ other.Get()); }

	template<typename U>
	constexpr Byte operator<<(Primitive<U> const& other) const noexcept { return Byte(m_value << other.Get()); }

	template<typename U>
	constexpr Byte operator>>(Primitive<U> const& other) const noexcept { return Byte(m_value >> other.Get()); }

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
	friend std::istream& operator>>(std::istream& lhs, Byte& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const Byte& rhs) { return lhs << rhs.m_value; }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Byte; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Byte* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Byte* const value) const override;
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

	// Exposed expressions
	static constexpr Byte MaxValue() { return Byte(static_cast<unsigned char>(0xFF)); }
	static constexpr Byte MinValue() { return Byte(static_cast<unsigned char>(0x0)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator+(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator-(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator*(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator/(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator%(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator&(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator|(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator^(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator<<(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Byte operator>>(T const& lhs, Byte const& rhs) noexcept
{
	return Byte(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, Byte const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator!=(T const& lhs, Byte const& rhs) noexcept
{
	return lhs != rhs.Get();
}