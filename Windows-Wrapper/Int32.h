#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "ValueTypeTraits.h"

class Int32 : public IComparable<Int32>, public IConvertible, public IEquatable<Int32>
{
protected:

	int32_t m_value;

public:

	constexpr Int32() noexcept : m_value() {}
	constexpr Int32(int32_t const& value) noexcept : m_value(value) {}
	constexpr Int32(Int32 const& other) noexcept : m_value(other.Get()) {}
	constexpr Int32(Int32&&) = default;
	constexpr Int32& operator=(Int32 const&) = default;
	constexpr Int32& operator=(Int32&&) = default;

	constexpr static Int32 From(int32_t const& other) noexcept { return Int32(other); }
	constexpr int32_t const& Get() const noexcept { return m_value; }
	
	constexpr Int32 const& operator+() const noexcept { return *this; }
	constexpr Int32 operator-() const noexcept { return Int32(-m_value); }
	constexpr Int32 operator~() const noexcept { return Int32(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	constexpr Int32& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr Int32 operator++(int) noexcept
	{
		return Int32(m_value++);
	}

	constexpr Int32& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr Int32 operator--(int) noexcept
	{
		return Int32(m_value--);
	}

	constexpr Int32& operator+=(int32_t const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	constexpr Int32& operator+=(Int32 const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	constexpr Int32& operator-=(int32_t const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	constexpr Int32& operator-=(Int32 const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	constexpr Int32& operator*=(int32_t const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	constexpr Int32& operator*=(Int32 const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	constexpr Int32& operator/=(int32_t const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	constexpr Int32& operator/=(Int32 const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	constexpr Int32& operator%=(int32_t const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	constexpr Int32& operator%=(Int32 const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	constexpr Int32& operator<<=(int32_t const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	constexpr Int32& operator<<=(Int32 const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	constexpr Int32& operator>>=(int32_t const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	constexpr Int32& operator>>=(Int32 const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	constexpr Int32& operator&=(int32_t const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	constexpr Int32& operator&=(Int32 const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	constexpr Int32& operator|=(int32_t const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	constexpr Int32& operator|=(Int32 const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	constexpr Int32& operator^=(int32_t const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	constexpr Int32& operator^=(Int32 const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	constexpr explicit operator Int32() const noexcept { return Int32(static_cast<int32_t>(m_value)); }
	friend std::istream& operator>>(std::istream& lhs, Int32& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, Int32& rhs) { return lhs << rhs.m_value; }

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Int32; };
	inline int GetHashCode() const noexcept override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Int32* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Int32* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	static constexpr Int32 MaxValue() { return Int32(0x7fffffff); }
	static constexpr Int32 MinValue() { return Int32(0x80000000); }
};

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator+(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() + rhs);
}
 
template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator+(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs + rhs.Get());
}

constexpr auto operator+(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator-(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() - rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator-(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs - rhs.Get());
}

constexpr auto operator-(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator*(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() * rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator*(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs * rhs.Get());
}

constexpr auto operator*(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator/(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() / rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator/(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs / rhs.Get());
}

constexpr auto operator/(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator%(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() % rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator%(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs % rhs.Get());
}

constexpr auto operator%(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator&(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() & rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator&(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs & rhs.Get());
}

constexpr auto operator&(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator|(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() | rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator|(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs | rhs.Get());
}

constexpr auto operator|(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator^(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() ^ rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator^(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs ^ rhs.Get());
}

constexpr auto operator^(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator<<(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() << rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator<<(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs << rhs.Get());
}

constexpr auto operator<<(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator>>(Int32 const& lhs, T const& rhs) noexcept
{
	return Int32(lhs.Get() >> rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr Int32 operator>>(T const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs >> rhs.Get());
}

constexpr auto operator>>(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return Int32(lhs.Get() >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator==(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() == rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator==(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs == rhs.Get();
}

constexpr bool operator==(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator!=(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() != rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator!=(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs != rhs.Get();
}

constexpr bool operator!=(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() != rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator<(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() < rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator<(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs < rhs.Get();
}

constexpr bool operator<(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator<=(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() <= rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator<=(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs <= rhs.Get();
}

constexpr bool operator<=(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() <= rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator>(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() > rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator>(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs > rhs.Get();
}

constexpr bool operator>(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() > rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator>=(Int32 const& lhs, T const& rhs) noexcept
{
	return lhs.Get() >= rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<int32_t, T>::value || is_promotion<T, int32_t>::value>>
constexpr bool operator>=(T const& lhs, Int32 const& rhs) noexcept
{
	return lhs >= rhs.Get();
}

constexpr bool operator>=(Int32 const& lhs, Int32 const& rhs) noexcept
{
	return lhs.Get() >= rhs.Get();
}