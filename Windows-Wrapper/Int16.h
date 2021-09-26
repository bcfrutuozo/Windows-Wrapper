#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Int16 final: public Primitive<short>, public IComparable<Int16>, public IConvertible, public IEquatable<Int16>
{
public:

	constexpr Int16() noexcept : Primitive() {}
	
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16(const U& value) noexcept : Primitive(value) {}
	
	template<typename U>
	constexpr Int16(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Int16(const Int16& value) noexcept : Primitive(value) { }

	Int16(Int16&&) = default;
	Int16& operator=(Int16 const&) = default;
	Int16& operator=(Int16&&) = default;

	constexpr Int16 const& operator+() const noexcept { return *this; }
	constexpr Int16 operator-() const noexcept { return Int16(-m_value); }
	constexpr Int16 operator~() const noexcept { return Int16(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr Int16& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr Int16 operator++(int) noexcept
	{
		return Int16(m_value++);
	}

	constexpr Int16& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr Int16 operator--(int) noexcept
	{
		return Int16(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Int16& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int16& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int16& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Int16& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<short>::value&& std::is_integral<U>::value>>
	constexpr Int16& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator+(U const& other) noexcept { return Int16(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator-(U const& other) noexcept { return Int16(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator*(U const& other) noexcept { return Int16(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator/(U const& other) noexcept { return Int16(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator%(U const& other) noexcept { return Int16(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator&(U const& other) noexcept { return Int16(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator|(U const& other) noexcept { return Int16(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator^(U const& other) noexcept { return Int16(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator<<(U const& other) noexcept { return Int16(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator>>(U const& other) noexcept { return Int16(m_value >> other); }

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
	constexpr Int16 operator+(Primitive<U> const& other) const noexcept { return Int16(m_value + other.Get()); }

	template<typename U>
	constexpr Int16 operator-(Primitive<U> const& other) const noexcept { return Int16(m_value - other.Get()); }

	template<typename U>
	constexpr Int16 operator*(Primitive<U> const& other) const noexcept { return Int16(m_value * other.Get()); }

	template<typename U>
	constexpr Int16 operator/(Primitive<U> const& other) const noexcept { return Int16(m_value / other.Get()); }

	template<typename U>
	constexpr Int16 operator%(Primitive<U> const& other) const noexcept { return Int16(m_value % other.Get()); }

	template<typename U>
	constexpr Int16 operator&(Primitive<U> const& other) const noexcept { return Int16(m_value & other.Get()); }

	template<typename U>
	constexpr Int16 operator|(Primitive<U> const& other) const noexcept { return Int16(m_value | other.Get()); }

	template<typename U>
	constexpr Int16 operator^(Primitive<U> const& other) const noexcept { return Int16(m_value ^ other.Get()); }

	template<typename U>
	constexpr Int16 operator<<(Primitive<U> const& other) const noexcept { return Int16(m_value << other.Get()); }

	template<typename U>
	constexpr Int16 operator>>(Primitive<U> const& other) const noexcept { return Int16(m_value >> other.Get()); }

	template<typename U>
	constexpr Int16 operator==(Primitive<U> const& other) const noexcept { return Int16(m_value == other.Get()); }

	template<typename U>
	constexpr Int16 operator!=(Primitive<U> const& other) const noexcept { return Int16(m_value != other.Get()); }

	template<typename U>
	constexpr Int16 operator<(Primitive<U> const& other) const noexcept { return Int16(m_value < other.Get()); }

	template<typename U>
	constexpr Int16 operator<=(Primitive<U> const& other) const noexcept { return Int16(m_value <= other.Get()); }

	template<typename U>
	constexpr Int16 operator>(Primitive<U> const& other) const noexcept { return Int16(m_value > other.Get()); }

	template<typename U>
	constexpr Int16 operator>=(Primitive<U> const& other) const noexcept { return Int16(m_value >= other.Get()); }

	// Output/input stream operator
	friend std::istream& operator>>(std::istream& lhs, Int16& const rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, Int16& const rhs) { return lhs << rhs.m_value; }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Int16; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Int16* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Int16* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Char ToChar(IFormatProvider* provider) const override;
	Byte ToByte(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	UInt16 ToUInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	// Exposed expressions
	static constexpr Int16 MaxValue() { return Int16(static_cast<short>(0x7FFF)); }
	static constexpr Int16 MinValue() { return Int16(static_cast<short>(0x8000)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator+(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator-(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator*(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator/(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator%(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator&(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator|(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator^(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator<<(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Int16 operator>>(T const& lhs, Int16 const& rhs) noexcept
{
	return Int16(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator!=(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs != rhs.Get();
}

//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator<(T const& lhs, Int16 const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}
//
//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator<=(T const& lhs, Int16 const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}
//
//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator>(T const& lhs, Int16 const& rhs) noexcept
//{
//	return lhs > rhs.Get();
//}
//
//template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
//constexpr bool operator>=(T const& lhs, Int16 const& rhs) noexcept
//{
//	return lhs >= rhs.Get();
//}