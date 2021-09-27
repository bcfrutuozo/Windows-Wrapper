#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class SByte final : public Primitive<signed char>, public IComparable<SByte>, public IConvertible, public IEquatable<SByte>
{
public:

	constexpr SByte() noexcept : Primitive() {}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte(const U& const value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr SByte(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr SByte(const SByte& value) noexcept : Primitive(value) { }

	SByte(SByte&&) = default;
	SByte& operator=(SByte const&) = default;
	SByte& operator=(SByte&&) = default;

	constexpr SByte const& operator+() const noexcept { return *this; }
	constexpr SByte operator-() const noexcept { return SByte(-m_value); }
	constexpr SByte operator~() const noexcept { return SByte(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr SByte& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr SByte operator++(int) noexcept
	{
		return SByte(m_value++);
	}

	constexpr SByte& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr SByte operator--(int) noexcept
	{
		return SByte(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr SByte& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr SByte& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr SByte& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr SByte& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<signed char>::value&& std::is_integral<U>::value>>
	constexpr SByte& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator+(U const& other) noexcept { return SByte(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator-(U const& other) noexcept { return SByte(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator*(U const& other) noexcept { return SByte(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator/(U const& other) noexcept { return SByte(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator%(U const& other) noexcept { return SByte(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator&(U const& other) noexcept { return SByte(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator|(U const& other) noexcept { return SByte(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator^(U const& other) noexcept { return SByte(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator<<(U const& other) noexcept { return SByte(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr SByte operator>>(U const& other) noexcept { return SByte(m_value >> other); }

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
	constexpr SByte operator+(Primitive<U> const& other) const noexcept { return SByte(m_value + other.Get()); }

	template<typename U>
	constexpr SByte operator-(Primitive<U> const& other) const noexcept { return SByte(m_value - other.Get()); }

	template<typename U>
	constexpr SByte operator*(Primitive<U> const& other) const noexcept { return SByte(m_value * other.Get()); }

	template<typename U>
	constexpr SByte operator/(Primitive<U> const& other) const noexcept { return SByte(m_value / other.Get()); }

	template<typename U>
	constexpr SByte operator%(Primitive<U> const& other) const noexcept { return SByte(m_value % other.Get()); }

	template<typename U>
	constexpr SByte operator&(Primitive<U> const& other) const noexcept { return SByte(m_value & other.Get()); }

	template<typename U>
	constexpr SByte operator|(Primitive<U> const& other) const noexcept { return SByte(m_value | other.Get()); }

	template<typename U>
	constexpr SByte operator^(Primitive<U> const& other) const noexcept { return SByte(m_value ^ other.Get()); }

	template<typename U>
	constexpr SByte operator<<(Primitive<U> const& other) const noexcept { return SByte(m_value << other.Get()); }

	template<typename U>
	constexpr SByte operator>>(Primitive<U> const& other) const noexcept { return SByte(m_value >> other.Get()); }

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
	friend std::istream& operator>>(std::istream& lhs, SByte& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const SByte& rhs) { return lhs << rhs.m_value; }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::SByte; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const SByte* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const SByte* const value) const override;
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
	static constexpr SByte MaxValue() { return SByte(static_cast<signed char>(0x7F)); }
	static constexpr SByte MinValue() { return SByte(static_cast<signed char>(0x80)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator+(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator-(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator*(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator/(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator%(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator&(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator|(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator^(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator<<(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr SByte operator>>(T const& lhs, SByte const& rhs) noexcept
{
	return SByte(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, SByte const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator!=(T const& lhs, SByte const& rhs) noexcept
{
	return lhs != rhs.Get();
}

//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator<(T const& lhs, SByte const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}

//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator<=(T const& lhs, SByte const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}
//
//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator>(T const& lhs, SByte const& rhs) noexcept
//{
//	return lhs > rhs.Get();
//}
//
//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator>=(T const& lhs, SByte const& rhs) noexcept
//{
//	return lhs >= rhs.Get();
//}