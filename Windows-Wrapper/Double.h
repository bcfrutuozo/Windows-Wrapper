#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

class Boolean;

class Double final : public Primitive<long double>, public IComparable<Double>, public IConvertible, public IEquatable<Double>
{
public:

	constexpr Double() noexcept : Primitive() {}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double(const U& value) noexcept : Primitive(value) {}

	template<typename U>
	constexpr Double(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Double(const Double& value) noexcept : Primitive(value) { }

	Double(Double&&) = default;
	Double& operator=(Double const&) = default;
	Double& operator=(Double&&) = default;

	constexpr Double const& operator+() const noexcept { return *this; }
	constexpr long double operator!() const noexcept { return !m_value; }

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double& operator+=(U const& other) noexcept {
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}


	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Double& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Double& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Double& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Double& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<long double>::value&& std::is_integral<U>::value>>
	constexpr Double& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator+(U const& other) noexcept { return Double(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator-(U const& other) noexcept { return Double(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator*(U const& other) noexcept { return Double(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator/(U const& other) noexcept { return Double(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator%(U const& other) noexcept { return Double(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator&(U const& other) noexcept { return Double(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator|(U const& other) noexcept { return Double(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator^(U const& other) noexcept { return Double(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator<<(U const& other) noexcept { return Double(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Double operator>>(U const& other) noexcept { return Double(m_value >> other); }

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
	constexpr Double operator+(Primitive<U> const& other) const noexcept { return Double(m_value + other.Get()); }

	template<typename U>
	constexpr Double operator-(Primitive<U> const& other) const noexcept { return Double(m_value - other.Get()); }

	template<typename U>
	constexpr Double operator*(Primitive<U> const& other) const noexcept { return Double(m_value * other.Get()); }

	template<typename U>
	constexpr Double operator/(Primitive<U> const& other) const noexcept { return Double(m_value / other.Get()); }

	template<typename U>
	constexpr Double operator%(Primitive<U> const& other) const noexcept { return Double(m_value % other.Get()); }

	template<typename U>
	constexpr Double operator&(Primitive<U> const& other) const noexcept { return Double(m_value & other.Get()); }

	template<typename U>
	constexpr Double operator|(Primitive<U> const& other) const noexcept { return Double(m_value | other.Get()); }

	template<typename U>
	constexpr Double operator^(Primitive<U> const& other) const noexcept { return Double(m_value ^ other.Get()); }

	template<typename U>
	constexpr Double operator<<(Primitive<U> const& other) const noexcept { return Double(m_value << other.Get()); }

	template<typename U>
	constexpr Double operator>>(Primitive<U> const& other) const noexcept { return Double(m_value >> other.Get()); }

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
	friend std::istream& operator>>(std::istream& lhs, Double& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, const Double& rhs) { return lhs << rhs.m_value; }

	// Logical AND/OR operators (Double exclusive)
	constexpr long double operator&&(long double const& other) noexcept { return m_value && other; }
	constexpr long double operator||(long double const& other) noexcept { return m_value || other; }
	constexpr long double operator&&(Double const& other) noexcept { return m_value && other.Get(); }
	constexpr long double operator||(Double const& other) noexcept { return m_value || other.Get(); }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Double; };
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Double* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Double* const value) const override;
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
	static constexpr Double MinValue() { return static_cast<long double>(-1.7976931348623157e+308L); }
	static constexpr Double Epsilon() { return static_cast<long double>(4.9406564584124654e-324L); }
	static constexpr Double MaxValue() { return static_cast<long double>(1.7976931348623157e+308L); };
	static constexpr Double PositiveInfinity() { return std::numeric_limits<long double>::infinity(); }
	static constexpr Double NegativeInfinity() { return -std::numeric_limits<long double>::infinity(); }
	static constexpr Double NaN() { return std::numeric_limits<long double>::quiet_NaN(); }
	static constexpr bool IsInfinity(const Double& d) { return (*(long*)(&d) & 0x7FFFFFFFFFFFFFFF) == 0x7FF0000000000000; }
	static constexpr bool IsPositiveInfinity(const Double& d) { return d == PositiveInfinity(); }
	static constexpr bool IsNegativeInfinity(const Double& d) { return d == NegativeInfinity(); }
	static constexpr bool IsNegative(const Double& d) { return (*(long long int*)(&d) & 0x8000000000000000) == 0x8000000000000000; }
	static constexpr bool IsNaN(const Double& d) { return (*(long long int*)(&d) & 0x7FFFFFFFFFFFFFFFL) > 0x7FF0000000000000L; }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator+(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator-(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator*(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator/(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator%(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator&(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator|(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator^(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator<<(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Double operator>>(T const& lhs, Double const& rhs) noexcept
{
	return Double(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr long double operator==(T const& lhs, Double const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr long double operator!=(T const& lhs, Double const& rhs) noexcept
{
	return lhs != rhs.Get();
}