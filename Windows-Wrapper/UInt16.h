#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class UInt16 final: public Primitive<unsigned short>, public IComparable<UInt16>, public IConvertible, public IEquatable<UInt16>
{
public:

	constexpr UInt16() noexcept : Primitive() {}
	
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16(const U& value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr UInt16(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr UInt16(const UInt16& value) noexcept : Primitive(value) { }

	UInt16(UInt16&&) = default;
	UInt16& operator=(UInt16 const&) = default;
	UInt16& operator=(UInt16&&) = default;

	constexpr UInt16 const& operator+() const noexcept { return *this; }
	// operator-() cannot be used with unsigned int because it can lead to undefined behavior
	constexpr UInt16 operator~() const noexcept { return UInt16(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr UInt16& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr UInt16 operator++(int) noexcept
	{
		return UInt16(m_value++);
	}

	constexpr UInt16& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr UInt16 operator--(int) noexcept
	{
		return UInt16(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr UInt16& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr UInt16& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr UInt16& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr UInt16& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<unsigned short>::value&& std::is_integral<U>::value>>
	constexpr UInt16& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator+(U const& other) noexcept { return UInt16(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator-(U const& other) noexcept { return UInt16(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator*(U const& other) noexcept { return UInt16(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator/(U const& other) noexcept { return UInt16(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator%(U const& other) noexcept { return UInt16(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator&(U const& other) noexcept { return UInt16(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator|(U const& other) noexcept { return UInt16(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator^(U const& other) noexcept { return UInt16(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator<<(U const& other) noexcept { return UInt16(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr UInt16 operator>>(U const& other) noexcept { return UInt16(m_value >> other); }

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
	constexpr UInt16 operator+(Primitive<U> const& other) const noexcept { return UInt16(m_value + other.Get()); }

	template<typename U>
	constexpr UInt16 operator-(Primitive<U> const& other) const noexcept { return UInt16(m_value - other.Get()); }

	template<typename U>
	constexpr UInt16 operator*(Primitive<U> const& other) const noexcept { return UInt16(m_value * other.Get()); }

	template<typename U>
	constexpr UInt16 operator/(Primitive<U> const& other) const noexcept { return UInt16(m_value / other.Get()); }

	template<typename U>
	constexpr UInt16 operator%(Primitive<U> const& other) const noexcept { return UInt16(m_value % other.Get()); }

	template<typename U>
	constexpr UInt16 operator&(Primitive<U> const& other) const noexcept { return UInt16(m_value & other.Get()); }

	template<typename U>
	constexpr UInt16 operator|(Primitive<U> const& other) const noexcept { return UInt16(m_value | other.Get()); }

	template<typename U>
	constexpr UInt16 operator^(Primitive<U> const& other) const noexcept { return UInt16(m_value ^ other.Get()); }

	template<typename U>
	constexpr UInt16 operator<<(Primitive<U> const& other) const noexcept { return UInt16(m_value << other.Get()); }

	template<typename U>
	constexpr UInt16 operator>>(Primitive<U> const& other) const noexcept { return UInt16(m_value >> other.Get()); }

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
	friend std::istream& operator>>(std::istream& lhs, UInt16& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const UInt16& rhs) { return lhs << rhs.m_value; }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::UInt16; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const UInt16* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const UInt16* const value) const override;
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
	static constexpr UInt16 MaxValue() { return UInt16(static_cast<unsigned short>(0xFFFF)); }
	static constexpr UInt16 MinValue() { return UInt16(static_cast<unsigned short>(0x0)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator+(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator-(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator*(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator/(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator%(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator&(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator|(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator^(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator<<(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr UInt16 operator>>(T const& lhs, UInt16 const& rhs) noexcept
{
	return UInt16(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, UInt16 const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator!=(T const& lhs, UInt16 const& rhs) noexcept
{
	return lhs != rhs.Get();
}