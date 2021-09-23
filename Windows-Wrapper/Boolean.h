#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "ValueTypeTraits.h"

#include "Int32.h"
//#include "String.h"

class Boolean : public IComparable<Boolean>, public IConvertible, public IEquatable<Boolean>
{
protected:

	bool m_value;

public:

	constexpr Boolean() noexcept : m_value() {}
	constexpr Boolean(bool const& value) noexcept : m_value(value) {}
	constexpr Boolean(Boolean const& other) noexcept : m_value(other.Get()) {}
	constexpr Boolean(Boolean&&) = default;
	constexpr Boolean& operator=(Boolean const&) = default;
	constexpr Boolean& operator=(Boolean&&) = default;

	constexpr static Boolean From(bool const& other) noexcept { return Boolean(other); }
	constexpr bool const& Get() const noexcept { return m_value; }

	constexpr Boolean const& operator+() const noexcept { return *this; }
	constexpr Boolean operator-() const noexcept { return Boolean(-m_value); }
	constexpr Boolean operator~() const noexcept { return Boolean(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	constexpr Boolean& operator+=(bool const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	constexpr Boolean& operator+=(Boolean const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	constexpr Boolean& operator-=(bool const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	constexpr Boolean& operator-=(Boolean const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	constexpr Boolean& operator*=(bool const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	constexpr Boolean& operator*=(Boolean const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	constexpr Boolean& operator/=(bool const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	constexpr Boolean& operator/=(Boolean const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	constexpr Boolean& operator%=(bool const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	constexpr Boolean& operator%=(Boolean const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	constexpr Boolean& operator<<=(bool const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	constexpr Boolean& operator<<=(Boolean const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	constexpr Boolean& operator>>=(bool const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	constexpr Boolean& operator>>=(Boolean const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	constexpr Boolean& operator&=(bool const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	constexpr Boolean& operator&=(Boolean const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	constexpr Boolean& operator|=(bool const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	constexpr Boolean& operator|=(Boolean const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	constexpr Boolean& operator^=(bool const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	constexpr Boolean& operator^=(Boolean const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	constexpr explicit operator Boolean() const noexcept { return Boolean(static_cast<bool>(m_value)); }
	friend std::istream& operator>>(std::istream& lhs, Boolean& rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream& lhs, Boolean& rhs) { return lhs << rhs.m_value; }

	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Boolean; };
	inline int GetHashCode() const noexcept override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Boolean* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Boolean* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	static constexpr Int32 True() { return 1; }
	static constexpr Int32 False() { return 0; }
};

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator+(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() + rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator+(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs + rhs.Get());
}

constexpr auto operator+(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator-(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() - rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator-(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs - rhs.Get());
}

constexpr auto operator-(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator*(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() * rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator*(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs * rhs.Get());
}

constexpr auto operator*(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator/(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() / rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator/(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs / rhs.Get());
}

constexpr auto operator/(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator%(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() % rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator%(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs % rhs.Get());
}

constexpr auto operator%(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator&(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() & rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator&(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs & rhs.Get());
}

constexpr auto operator&(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator|(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() | rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator|(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs | rhs.Get());
}

constexpr auto operator|(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator^(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() ^ rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator^(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs ^ rhs.Get());
}

constexpr auto operator^(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator<<(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() << rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator<<(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs << rhs.Get());
}

constexpr auto operator<<(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator>>(Boolean const& lhs, T const& rhs) noexcept
{
	return Boolean(lhs.Get() >> rhs);
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr Boolean operator>>(T const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs >> rhs.Get());
}

constexpr auto operator>>(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return Boolean(lhs.Get() >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator==(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() == rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator==(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs == rhs.Get();
}

constexpr bool operator==(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() == rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator!=(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() != rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator!=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs != rhs.Get();
}

constexpr bool operator!=(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() != rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator<(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() < rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator<(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs < rhs.Get();
}

constexpr bool operator<(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() < rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator<=(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() <= rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator<=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs <= rhs.Get();
}

constexpr bool operator<=(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() <= rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator>(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() > rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator>(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs > rhs.Get();
}

constexpr bool operator>(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() > rhs.Get();
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator>=(Boolean const& lhs, T const& rhs) noexcept
{
	return lhs.Get() >= rhs;
}

template<typename T, typename = std::enable_if_t<std::is_same<bool, T>::value || is_promotion<T, bool>::value>>
constexpr bool operator>=(T const& lhs, Boolean const& rhs) noexcept
{
	return lhs >= rhs.Get();
}

constexpr bool operator>=(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() >= rhs.Get();
}

constexpr bool operator&&(Boolean const& lhs, bool const& rhs) noexcept
{
	return lhs.Get() && rhs;
}

constexpr bool operator&&(bool const& lhs, Boolean const& rhs) noexcept
{
	return lhs && rhs.Get();
}

constexpr bool operator&&(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() && rhs.Get();
}

constexpr bool operator||(Boolean const& lhs, bool const& rhs) noexcept
{
	return lhs.Get() || rhs;
}

constexpr bool operator||(bool const& lhs, Boolean const& rhs) noexcept
{
	return lhs || rhs.Get();
}

constexpr bool operator||(Boolean const& lhs, Boolean const& rhs) noexcept
{
	return lhs.Get() || rhs.Get();
}