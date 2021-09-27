#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Single final : public Primitive<float>, public IComparable<Single>, public IConvertible, public IEquatable<Single>
{
public:

	constexpr Single() noexcept : Primitive() {}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single(const U& value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr Single(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Single(const Single& value) noexcept : Primitive(value) { }

	Single(Single&&) = default;
	Single& operator=(Single const&) = default;
	Single& operator=(Single&&) = default;

	constexpr Single const& operator+() const noexcept { return *this; }
	constexpr float operator!() const noexcept { return !m_value; }

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single& operator+=(U const& other) noexcept {
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Single& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Single& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Single& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Single& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<float>::value&& std::is_integral<U>::value>>
	constexpr Single& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator+(U const& other) noexcept { return Single(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator-(U const& other) noexcept { return Single(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator*(U const& other) noexcept { return Single(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator/(U const& other) noexcept { return Single(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator%(U const& other) noexcept { return Single(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator&(U const& other) noexcept { return Single(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator|(U const& other) noexcept { return Single(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator^(U const& other) noexcept { return Single(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator<<(U const& other) noexcept { return Single(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Single operator>>(U const& other) noexcept { return Single(m_value >> other); }

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
	constexpr Single operator+(Primitive<U> const& other) const noexcept { return Single(m_value + other.Get()); }

	template<typename U>
	constexpr Single operator-(Primitive<U> const& other) const noexcept { return Single(m_value - other.Get()); }

	template<typename U>
	constexpr Single operator*(Primitive<U> const& other) const noexcept { return Single(m_value * other.Get()); }

	template<typename U>
	constexpr Single operator/(Primitive<U> const& other) const noexcept { return Single(m_value / other.Get()); }

	template<typename U>
	constexpr Single operator%(Primitive<U> const& other) const noexcept { return Single(m_value % other.Get()); }

	template<typename U>
	constexpr Single operator&(Primitive<U> const& other) const noexcept { return Single(m_value & other.Get()); }

	template<typename U>
	constexpr Single operator|(Primitive<U> const& other) const noexcept { return Single(m_value | other.Get()); }

	template<typename U>
	constexpr Single operator^(Primitive<U> const& other) const noexcept { return Single(m_value ^ other.Get()); }

	template<typename U>
	constexpr Single operator<<(Primitive<U> const& other) const noexcept { return Single(m_value << other.Get()); }

	template<typename U>
	constexpr Single operator>>(Primitive<U> const& other) const noexcept { return Single(m_value >> other.Get()); }

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
	friend std::istream& operator>>(std::istream& lhs, Single& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const Single& rhs) { return lhs << rhs.m_value; }

	// Logical AND/OR operators (Single exclusive)
	constexpr float operator&&(float const& other) noexcept { return m_value && other; }
	constexpr float operator||(float const& other) noexcept { return m_value || other; }
	constexpr float operator&&(Single const& other) noexcept { return m_value && other.Get(); }
	constexpr float operator||(Single const& other) noexcept { return m_value || other.Get(); }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Single; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Single* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Single* const value) const override;
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
	static constexpr Single MinValue() { return static_cast<float>(-3.40282346638528859e+38f); }
	static constexpr Single Epsilon() { return static_cast<float>(1.4e-45f); }
	static constexpr Single MaxValue() { return static_cast<float>(3.40282346638528859e+38f); };
	static constexpr Single PositiveInfinity() { return std::numeric_limits<float>::infinity(); }
	static constexpr Single NegativeInfinity() { return -std::numeric_limits<float>::infinity(); }
	static constexpr Single NaN() { return std::numeric_limits<float>::quiet_NaN(); }
	static constexpr bool IsInfinity(const Single& f) { return (*(int*)(&f) & 0x7FFFFFFF) == 0x7F800000; }
	static constexpr bool IsPositiveInfinity(const Single& f) { return *(int*)(&f) == 0x7F800000; }
	static constexpr bool IsNegativeInfinity(const Single& f) { return *(int*)(&f) == ((int)0xFF800000); }
	static constexpr bool IsNaN(const Single& f) { return (*(int*)(&f) & 0x7FFFFFFF) > 0x7F800000; }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator+(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator-(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator*(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator/(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator%(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator&(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator|(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator^(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator<<(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Single operator>>(T const& lhs, Single const& rhs) noexcept
{
	return Single(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr float operator==(T const& lhs, Single const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr float operator!=(T const& lhs, Single const& rhs) noexcept
{
	return lhs != rhs.Get();
}