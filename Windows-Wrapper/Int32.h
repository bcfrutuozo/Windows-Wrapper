#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"

#include <iosfwd>
#include <type_traits>

class Int32 : public IConvertible, public IEquatable<Int32>, public IComparable<Int32>
{
protected:

	int32_t m_value;

public:

	constexpr Int32() noexcept
		:
		m_value()
	{}

	constexpr Int32(int32_t const& value) noexcept
		:
		m_value(value)
	{}

	constexpr Int32(Int32 const& other) noexcept
		:
		m_value(other.get())
	{}

	constexpr static Int32 from(int32_t const& other) noexcept
	{
		return Int32(int32_t(other));
	}

	Int32(Int32&&) = default;
	Int32& operator=(Int32 const&) = default;
	Int32& operator=(Int32&&) = default;

	constexpr int32_t const& get() const noexcept { return m_value; }
	constexpr Int32 const& operator+() const noexcept { return *this; }
	constexpr Int32 operator-() const noexcept { return Int32(-m_value); }
	constexpr Int32 operator~() const noexcept { return Int32(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	Int32& operator++() noexcept
	{
		++m_value;
		return *this;
	}
	Int32 operator++(int) noexcept
	{
		return Int32(m_value++);
	}

	Int32& operator--() noexcept
	{
		--m_value;
		return *this;
	}
	Int32 operator--(int) noexcept
	{
		return Int32(m_value--);
	}

	Int32& operator+=(int32_t const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	Int32& operator+=(Int32 const& other) noexcept
	{
		m_value += other.get();
		return *this;
	}

	Int32& operator-=(int32_t const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	Int32& operator-=(Int32 const& other) noexcept
	{
		m_value -= other.get();
		return *this;
	}

	Int32& operator*=(int32_t const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	Int32& operator*=(Int32 const& other) noexcept
	{
		m_value *= other.get();
		return *this;
	}

	Int32& operator/=(int32_t const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	Int32& operator/=(Int32 const& other) noexcept
	{
		m_value /= other.get();
		return *this;
	}

	Int32& operator%=(int32_t const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	Int32& operator%=(Int32 const& other) noexcept
	{
		m_value %= other.get();
		return *this;
	}

	Int32& operator<<=(int32_t const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}
	Int32& operator<<=(Int32 const& other) noexcept
	{
		m_value <<= other.get();
		return *this;
	}

	Int32& operator>>=(int32_t const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	Int32& operator>>=(Int32 const& other) noexcept
	{
		m_value >>= other.get();
		return *this;
	}

	Int32& operator&=(int32_t const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	Int32& operator&=(Int32 const& other) noexcept
	{
		m_value &= other.get();
		return *this;
	}

	Int32& operator|=(int32_t const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	Int32& operator|=(Int32 const& other) noexcept
	{
		m_value |= other.get();
		return *this;
	}

	Int32& operator^=(int32_t const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	Int32& operator^=(Int32 const& other) noexcept
	{
		m_value ^= other.get();
		return *this;
	}

	constexpr explicit operator Int32() const noexcept
	{
		return Int32(static_cast<int32_t>(m_value));
	}

	friend std::istream& operator>>(std::istream& lhs, Int32& rhs)
	{
		return lhs >> rhs.m_value;
	}

	Int32 operator+(int32_t other) const noexcept { return Int32(m_value + other); }

	int CompareTo(const Object* const b) const override { return 0; }
	int CompareTo(const Int32* const b) const override { return 0; }
	bool Equals(const Object* const obj) const override { return true; }
	bool Equals(const Int32* const value) const override { return true; }
	Int32 ToInt32(IFormatProvider* provider) const override { return Int32(0); }
};

//template<>
//class ValueType<int32_t>
//{
//public:
//
//	int CompareTo(const Object* const obj) const
//	{
//		if (obj == nullptr) return false;
//
//		if (const auto value = dynamic_cast<const ValueType<int32_t>*>(obj))
//		{
//			return *this == *value;
//		}
//
//		if (const auto value = (int32_t*)(obj))
//		{
//			return m_value == *value;
//		}
//
//		throw ArgumentException("Arg_MustBeInt32");
//	}
//
//	int CompareTo(const int32_t* const value) const
//	{
//		if (value == nullptr) return false;
//		return m_value == *value;
//	}
//
//	bool Equals(const Object* const obj) const noexcept
//	{
//		return true;
//	}
//
//	bool Equals(const int32_t* const b) const noexcept
//	{
//		return true;
//	}
//
//	ValueType<int32_t> ToInt32(IFormatProvider* provider) const
//	{
//		return ValueType<int32_t>();
//	}
//};