//#pragma once
//
//#include "IEquatable.h"
//#include "IComparable.h"
//#include "IConvertible.h"
//
//class Boolean;
//class Char;
//class Int32;
//
//class Byte : public IComparable<Byte>, public IConvertible, public IEquatable<Byte>
//{
//private:
//
//	unsigned char m_value;
//
//public:
//
//	using value_type = unsigned char;
//
//	constexpr Byte() noexcept : m_value() {}
//	constexpr Byte(int const& value) noexcept : m_value(value) {}
//	constexpr Byte(byte const& value) noexcept : m_value(value) {}
//	constexpr Byte(Byte const& other) noexcept : m_value(other.Get()) {}
//	constexpr Byte(Byte&&) = default;
//	constexpr Byte& operator=(Byte const&) = default;
//	constexpr Byte& operator=(Byte&&) = default;
//	constexpr static Byte From(byte const& other) noexcept { return Byte(other); }
//	constexpr byte const& Get() const noexcept { return m_value; }
//
//	constexpr Byte const& operator+() const noexcept { return *this; }
//
//	constexpr Byte operator-() const noexcept { return Byte(-m_value); }
//
//	constexpr Byte operator~() const noexcept { return Byte(~m_value); }
//
//	// Boolean is forward declared, so this function cannot be constexpr
//	Boolean operator!() const noexcept;
//
//	constexpr Byte& operator++() noexcept
//	{
//		++m_value;
//		return *this;
//	}
//
//	constexpr Byte operator++(int) noexcept
//	{
//		return Byte(m_value++);
//	}
//
//	constexpr Byte& operator--() noexcept
//	{
//		--m_value;
//		return *this;
//	}
//
//	constexpr Byte operator--(int) noexcept
//	{
//		return Byte(m_value--);
//	}
//
//	constexpr Byte& operator+=(byte const& other) noexcept
//	{
//		m_value += other;
//		return *this;
//	}
//
//	constexpr Byte& operator+=(Byte const& other) noexcept
//	{
//		m_value += other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator-=(byte const& other) noexcept
//	{
//		m_value -= other;
//		return *this;
//	}
//
//	constexpr Byte& operator-=(Byte const& other) noexcept
//	{
//		m_value -= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator*=(byte const& other) noexcept
//	{
//		m_value *= other;
//		return *this;
//	}
//
//	constexpr Byte& operator*=(Byte const& other) noexcept
//	{
//		m_value *= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator/=(byte const& other) noexcept
//	{
//		m_value /= other;
//		return *this;
//	}
//
//	constexpr Byte& operator/=(Byte const& other) noexcept
//	{
//		m_value /= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator%=(byte const& other) noexcept
//	{
//		m_value %= other;
//		return *this;
//	}
//
//	constexpr Byte& operator%=(Byte const& other) noexcept
//	{
//		m_value %= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator<<=(byte const& other) noexcept
//	{
//		m_value <<= other;
//		return *this;
//	}
//
//	constexpr Byte& operator<<=(Byte const& other) noexcept
//	{
//		m_value <<= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator>>=(byte const& other) noexcept
//	{
//		m_value >>= other;
//		return *this;
//	}
//
//	constexpr Byte& operator>>=(Byte const& other) noexcept
//	{
//		m_value >>= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator&=(byte const& other) noexcept
//	{
//		m_value &= other;
//		return *this;
//	}
//
//	constexpr Byte& operator&=(Byte const& other) noexcept
//	{
//		m_value &= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator|=(byte const& other) noexcept
//	{
//		m_value |= other;
//		return *this;
//	}
//
//	constexpr Byte& operator|=(Byte const& other) noexcept
//	{
//		m_value |= other.Get();
//		return *this;
//	}
//
//	constexpr Byte& operator^=(byte const& other) noexcept
//	{
//		m_value ^= other;
//		return *this;
//	}
//
//	constexpr Byte& operator^=(Byte const& other) noexcept
//	{
//		m_value ^= other.Get();
//		return *this;
//	}
//
//	constexpr explicit operator Byte() const noexcept { return Byte(static_cast<byte>(m_value)); }
//	friend std::istream& operator>>(std::istream & lhs, Byte & rhs) { return lhs >> rhs.m_value; }
//	friend std::ostream& operator<<(std::ostream & lhs, Byte & rhs) { return lhs << rhs.m_value; }
//
//	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Byte; };
//	inline int GetHashCode() const override;
//	inline int CompareTo(const Object* const obj) const override;
//	inline int CompareTo(const Byte* const value) const override;
//	inline bool Equals(const Object* const obj) const override;
//	inline bool Equals(const Byte* const value) const override;
//	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
//	//inline String ToString() const noexcept override;
//	Boolean ToBoolean(IFormatProvider * provider) const override;
//	Int32 ToInt32(IFormatProvider * provider) const override;
//
//	static constexpr Byte MaxValue() { return Byte(static_cast<byte>(0xFF)); }
//	static constexpr Byte MinValue() { return Byte(static_cast<byte>(0x0)); }
//};