#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Int16 : public Primitive<short>, public IComparable<Int16>, public IConvertible, public IEquatable<Int16>
{
private:

	constexpr Int16(Primitive<short> const& value) : Primitive(value.Get()) { }

public:

	constexpr Int16() noexcept : Primitive() {}
	constexpr Int16(const short& value) noexcept : Primitive(value) { }
	constexpr Int16(const Int16& value) noexcept : Primitive(value) { }

	Int16(Int16&&) = default;
	Int16& operator=(Int16 const&) = default;
	Int16& operator=(Int16&&) = default;

	template<typename U, typename = std::enable_if_t<is_conversion<U, short>::value>>
	constexpr static Int16 From(U const& other) noexcept { return Int16(U(other)); }

	constexpr Int16 const& operator+() const noexcept { return *this; }
	constexpr Int16 operator-() const noexcept { return Int16(-m_value); }
	constexpr Int16 operator~() const noexcept { return Int16(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

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

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	Int16& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U>
	Int16& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	Int16& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U>
	Int16& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	Int16& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U>
	Int16& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	Int16& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U>
	Int16& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t< std::is_integral<short>::value&& std::is_integral<U>::value>>
	Int16& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	friend std::istream& operator>>(std::istream& lhs, Int16& const rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, Int16& const rhs) { return lhs << rhs.m_value; }

	/*Int16 operator+(Boolean const& other) const noexcept;*/

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Int16 operator+(U const& other) noexcept { return Int16(other + m_value); }

	template<typename U>
	constexpr Int16 operator+(Primitive<U> const& other) const noexcept { return Int16(other.Get() + m_value); }

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

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Int16; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Int16* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Int16* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	static constexpr Int16 MaxValue() { return Int16(static_cast<short>(0x7FFF)); }
	static constexpr Int16 MinValue() { return Int16(static_cast<short>(0x8000)); }
};

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
	return Int16(lhs - rhs.Get());
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

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator<(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator<=(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator>(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs > rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator>=(T const& lhs, Int16 const& rhs) noexcept
{
	return lhs >= rhs.Get();
}