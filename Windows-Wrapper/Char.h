#pragma once

#include "IEquatable.h"
#include "IComparable.h"
#include "IConvertible.h"
#include "Primitives.h"

enum class UnicodeCategory
{
	UppercaseLetter = 0,
	LowercaseLetter = 1,
	TitlecaseLetter = 2,
	ModifierLetter = 3,
	OtherLetter = 4,
	NonSpacingMark = 5,
	SpacingCombiningMark = 6,
	EnclosingMark = 7,
	DecimalDigitNumber = 8,
	LetterNumber = 9,
	OtherNumber = 10,
	SpaceSeparator = 11,
	LineSeparator = 12,
	ParagraphSeparator = 13,
	Control = 14,
	Format = 15,
	Surrogate = 16,
	PrivateUse = 17,
	ConnectorPunctuation = 18,
	DashPunctuation = 19,
	OpenPunctuation = 20,
	ClosePunctuation = 21,
	InitialQuotePunctuation = 22,
	FinalQuotePunctuation = 23,
	OtherPunctuation = 24,
	MathSymbol = 25,
	CurrencySymbol = 26,
	ModifierSymbol = 27,
	OtherSymbol = 28,
	OtherNotAssigned = 29,
};

class Boolean;
class Char;
class String;

class Char final: public Primitive<char>, public IComparable<Char>, public IConvertible, public IEquatable<Char>
{
private:

	// Returns true for all characters below or equal U+00ff, which is ASCII + Latin-1 Supplement.
	static constexpr bool IsLatin1(char ch) noexcept { return (ch <= '\x00ff'); }

	// Returns true for all characters below or equal U+007f, which is ASCII.
	static constexpr bool IsASCII(char ch) noexcept { return (ch <= '\x007f'); }

	// Unicode category values from Unicode U+0000 ~ U+00FF. Storing them in Byte[] array to save space.
	static constexpr byte CategoryForLatin1[256] = 
	{
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0000 - 0007
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0008 - 000F
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0010 - 0017
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0018 - 001F
		(byte)UnicodeCategory::SpaceSeparator, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation,    // 0020 - 0027
		(byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::DashPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation,    // 0028 - 002F
		(byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber,    // 0030 - 0037
		(byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::DecimalDigitNumber, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherPunctuation,    // 0038 - 003F
		(byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0040 - 0047
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0048 - 004F
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 0050 - 0057
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::ConnectorPunctuation,    // 0058 - 005F
		(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0060 - 0067
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0068 - 006F
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 0070 - 0077
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::OpenPunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::ClosePunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::Control,    // 0078 - 007F
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0080 - 0087
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0088 - 008F
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0090 - 0097
		(byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control, (byte)UnicodeCategory::Control,    // 0098 - 009F
		(byte)UnicodeCategory::SpaceSeparator, (byte)UnicodeCategory::OtherPunctuation, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::CurrencySymbol, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::OtherSymbol,    // 00A0 - 00A7
		(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::InitialQuotePunctuation, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::DashPunctuation, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::ModifierSymbol,    // 00A8 - 00AF
		(byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::MathSymbol, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::OtherSymbol, (byte)UnicodeCategory::OtherPunctuation,    // 00B0 - 00B7
		(byte)UnicodeCategory::ModifierSymbol, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::FinalQuotePunctuation, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherNumber, (byte)UnicodeCategory::OtherPunctuation,    // 00B8 - 00BF
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 00C0 - 00C7
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter,    // 00C8 - 00CF
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::MathSymbol,    // 00D0 - 00D7
		(byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::UppercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00D8 - 00DF
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00E0 - 00E7
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00E8 - 00EF
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::MathSymbol,    // 00F0 - 00F7
		(byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter, (byte)UnicodeCategory::LowercaseLetter,    // 00F8 - 00FF
	};

	static constexpr UnicodeCategory GetLatin1UnicodeCategory(char ch)
	{
		assert(IsLatin1(ch), "Char.GetLatin1UnicodeCategory(): ch should be <= 007f");
		return static_cast<UnicodeCategory>((CategoryForLatin1[static_cast<int>(ch)]));
	}

	static Boolean IsWhiteSpaceLatin1(char ch) noexcept;

public:

	constexpr Char() noexcept : Primitive() {}
	
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char(const U& value) noexcept : Primitive(value) {}
	
	template<typename U>
	constexpr Char(const Primitive<U>& value) noexcept : Primitive(value) { }

	constexpr Char(const Char& value) noexcept : Primitive(value) { }

	Char(Char&&) = default;
	Char& operator=(Char const&) = default;
	Char& operator=(Char&&) = default;

	constexpr Char const& operator+() const noexcept { return *this; }
	constexpr Char operator-() const noexcept { return Char(-m_value); }
	constexpr Char operator~() const noexcept { return Char(~m_value); }
	constexpr bool operator!() const noexcept { return !m_value; }

	// Increment/decrement operators
	constexpr Char& operator++() noexcept
	{
		++m_value;
		return *this;
	}

	constexpr Char operator++(int) noexcept
	{
		return Char(m_value++);
	}

	constexpr Char& operator--() noexcept
	{
		--m_value;
		return *this;
	}

	constexpr Char operator--(int) noexcept
	{
		return Char(m_value--);
	}

	// Assignment operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char& operator+=(U const& other) noexcept
	{
		m_value += other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char& operator-=(U const& other) noexcept
	{
		m_value -= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char& operator*=(U const& other) noexcept
	{
		m_value *= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char& operator/=(U const& other) noexcept
	{
		m_value /= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator%=(U const& other) noexcept
	{
		m_value %= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator<<=(U const& other) noexcept
	{
		m_value <<= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator>>=(U const& other) noexcept
	{
		m_value >>= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator&=(U const& other) noexcept
	{
		m_value &= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator|=(U const& other) noexcept
	{
		m_value |= other;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator^=(U const& other) noexcept
	{
		m_value ^= other;
		return *this;
	}

	// Assignment operators with Primitive<U> types
	template<typename U>
	constexpr Char& operator+=(Primitive<U> const& other) noexcept
	{
		m_value += other.Get();
		return *this;
	}

	template<typename U>
	constexpr Char& operator-=(Primitive<U> const& other) noexcept
	{
		m_value -= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Char& operator*=(Primitive<U> const& other) noexcept
	{
		m_value *= other.Get();
		return *this;
	}

	template<typename U>
	constexpr Char& operator/=(Primitive<U> const& other) noexcept
	{
		m_value /= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator%=(Primitive<U> const& other) noexcept
	{
		m_value %= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator<<=(Primitive<U> const& other) noexcept
	{
		m_value <<= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator>>=(Primitive<U> const& other) noexcept
	{
		m_value >>= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator&=(Primitive<U> const& other) noexcept
	{
		m_value &= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator|=(Primitive<U> const& other) noexcept
	{
		m_value |= other.Get();
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_integral<char>::value&& std::is_integral<U>::value>>
	constexpr Char& operator^=(Primitive<U> const& other) noexcept
	{
		m_value ^= other.Get();
		return *this;
	}

	// Operators with fundamental types
	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator+(U const& other) noexcept { return Char(m_value + other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator-(U const& other) noexcept { return Char(m_value - other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator*(U const& other) noexcept { return Char(m_value * other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator/(U const& other) noexcept { return Char(m_value / other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator%(U const& other) noexcept { return Char(m_value % other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator&(U const& other) noexcept { return Char(m_value & other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator|(U const& other) noexcept { return Char(m_value | other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator^(U const& other) noexcept { return Char(m_value ^ other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator<<(U const& other) noexcept { return Char(m_value << other); }

	template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
	constexpr Char operator>>(U const& other) noexcept { return Char(m_value >> other); }

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
	constexpr Char operator+(Primitive<U> const& other) const noexcept { return Char(m_value + other.Get()); }

	template<typename U>
	constexpr Char operator-(Primitive<U> const& other) const noexcept { return Char(m_value - other.Get()); }

	template<typename U>
	constexpr Char operator*(Primitive<U> const& other) const noexcept { return Char(m_value * other.Get()); }

	template<typename U>
	constexpr Char operator/(Primitive<U> const& other) const noexcept { return Char(m_value / other.Get()); }

	template<typename U>
	constexpr Char operator%(Primitive<U> const& other) const noexcept { return Char(m_value % other.Get()); }

	template<typename U>
	constexpr Char operator&(Primitive<U> const& other) const noexcept { return Char(m_value & other.Get()); }

	template<typename U>
	constexpr Char operator|(Primitive<U> const& other) const noexcept { return Char(m_value | other.Get()); }

	template<typename U>
	constexpr Char operator^(Primitive<U> const& other) const noexcept { return Char(m_value ^ other.Get()); }

	template<typename U>
	constexpr Char operator<<(Primitive<U> const& other) const noexcept { return Char(m_value << other.Get()); }

	template<typename U>
	constexpr Char operator>>(Primitive<U> const& other) const noexcept { return Char(m_value >> other.Get()); }

	template<typename U>
	constexpr Char operator==(Primitive<U> const& other) const noexcept { return Char(m_value == other.Get()); }

	template<typename U>
	constexpr Char operator!=(Primitive<U> const& other) const noexcept { return Char(m_value != other.Get()); }

	template<typename U>
	constexpr Char operator<(Primitive<U> const& other) const noexcept { return Char(m_value < other.Get()); }

	template<typename U>
	constexpr Char operator<=(Primitive<U> const& other) const noexcept { return Char(m_value <= other.Get()); }

	template<typename U>
	constexpr Char operator>(Primitive<U> const& other) const noexcept { return Char(m_value > other.Get()); }

	template<typename U>
	constexpr Char operator>=(Primitive<U> const& other) const noexcept { return Char(m_value >= other.Get()); }

	// Output/input stream operator
	friend std::istream& operator>>(std::istream & lhs, Char& const rhs) { return lhs >> rhs.m_value; }
	friend std::ostream& operator<<(std::ostream & lhs, Char& const rhs) { return lhs << rhs.m_value; }

	// Interfaces and virtual functions implementation
	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::Char; }
	inline int GetHashCode() const override;
	inline int CompareTo(const Object* const obj) const override;
	inline int CompareTo(const Char* const value) const override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Char* const value) const override;
	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
	//inline String ToString() const noexcept override;
	Boolean ToBoolean(IFormatProvider* provider) const override;
	Char ToChar(IFormatProvider* provider) const override;
	Byte ToByte(IFormatProvider* provider) const override;
	Int16 ToInt16(IFormatProvider* provider) const override;
	UInt16 ToUInt16(IFormatProvider* provider) const override;
	Int32 ToInt32(IFormatProvider* provider) const override;

	//static String ToString(Char const& ch) noexcept;
	//static Char Parse(String const& s);
	//static Boolean TryParse(String s, Char& result) noexcept;
	static Boolean IsDigit(Char const& ch) noexcept;
	static Boolean CheckLetter(const UnicodeCategory& uc) noexcept;
	static Boolean IsLetter(Char const& ch) noexcept;
	static Boolean IsWhiteSpace(Char const& ch) noexcept;
	static Boolean IsUpper(Char const& ch) noexcept;
	static Boolean IsLower(Char const& ch) noexcept;
	static Boolean CheckPunctuation(const UnicodeCategory& uc) noexcept;
	static Boolean IsPunctuation(Char const& ch) noexcept;
	static Boolean CheckLetterOrDigit(const UnicodeCategory& uc) noexcept;
	static Boolean IsLetterOrDigit(Char ch) noexcept;
	static Char ToUpper(Char const& ch) noexcept;
	static Char ToLower(Char const& ch) noexcept;

	// Exposed expressions
	static constexpr Char MaxValue() { return Char(static_cast<char>(0xFFFF)); }
	static constexpr Char MinValue() { return Char(static_cast<char>(0x00)); }
};

// Operators for operations with fundamental types as first parameters
template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator+(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs + rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator-(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs - rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator*(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs * rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator/(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs / rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator%(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs % rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator&(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs & rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator|(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs | rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator^(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs ^ rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator<<(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs << rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr Char operator>>(T const& lhs, Char const& rhs) noexcept
{
	return Char(lhs >> rhs.Get());
}

template<typename T, typename = std::enable_if_t<std::is_fundamental<T>::value>>
constexpr bool operator==(T const& lhs, Char const& rhs) noexcept
{
	return lhs == rhs.Get();
}

template<typename T, std::is_fundamental<T>::value>
constexpr bool operator!=(T const& lhs, Char const& rhs) noexcept
{
	return lhs != rhs.Get();
}

//template<typename T, std::is_fundamental<T>::value>
//constexpr bool operator<(T const& lhs, Char const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}

//template<typename T, std::is_fundamental<T>::value>
//constexpr bool operator<=(T const& lhs, Char const& rhs) noexcept
//{
//	return lhs < rhs.Get();
//}
//
//template<typename T, std::is_fundamental<T>::value>
//constexpr bool operator>(T const& lhs, Char const& rhs) noexcept
//{
//	return lhs > rhs.Get();
//}
//
//template<typename T, std::is_fundamental<T>::value>
//constexpr bool operator>=(T const& lhs, Char const& rhs) noexcept
//{
//	return lhs >= rhs.Get();
//}