#include "Char.h"
#include "Boolean.h"
#include "Byte.h"
#include "Int16.h"
#include "UInt16.h"
#include "StringWrapper.h"
#include "Exceptions.h"

Boolean Char::IsWhiteSpaceLatin1(char ch) noexcept
{
	// There are characters which belong to UnicodeCategory.Control but are considered as white spaces.
	// We use code point comparisons for these characters here as a temporary fix.

	// U+0009 = <control> HORIZONTAL TAB
	// U+000a = <control> LINE FEED
	// U+000b = <control> VERTICAL TAB
	// U+000c = <contorl> FORM FEED
	// U+000d = <control> CARRIAGE RETURN
	// U+0085 = <control> NEXT LINE
	// U+00a0 = NO-BREAK SPACE
	if ((ch == ' ') || (ch >= '\x0009' && ch <= '\x000d') || ch == '\x00a0' || ch == '\x0085')
	{
		return true;
	}

	return false;
}

int Char::GetHashCode() const
{
	return static_cast<int>(m_value) | (static_cast<int>(m_value) << 16);
}

int Char::CompareTo(const Object* const obj) const
{
	if (obj == nullptr) return 1;

	if (const auto value = dynamic_cast<const Char*>(obj))
	{
		return m_value - value->Get();
	}

	if (const auto value = (char*)(obj))
	{
		return m_value - *value;
	}

	throw ArgumentException("Arg_MustBeChar");
}

int Char::CompareTo(const Char* const value) const
{
	if (value == nullptr) return 1;

	return m_value - value->Get();
}

bool Char::Equals(const Object* const obj) const
{
	if (obj == nullptr) return false;

	if (const auto value = dynamic_cast<const Char*>(obj))
	{
		return Get() == value->Get();
	}

	if (const auto value = (char*)(obj))
	{
		return Get() == *value;
	}

	return false;
}

bool Char::Equals(const Char* const b) const
{
	if (b == nullptr) return false;

	return Get() == b->Get();
}

Boolean Char::ToBoolean(IFormatProvider* provider) const
{
	//TODO:: CONVERT INT32 TO BOOLEAN
	return false;
}

Char Char::ToChar(IFormatProvider* provider) const
{
	return Char();
}

Byte Char::ToByte(IFormatProvider* provider) const
{
	return Byte();
}

Int16 Char::ToInt16(IFormatProvider* provider) const
{
	return 0;
}

UInt16 Char::ToUInt16(IFormatProvider* provider) const
{
	return UInt16();
}

Int32 Char::ToInt32(IFormatProvider* provider) const
{
	return 0;
}

//String Char::ToString(Char const& ch) noexcept
//{
//	return String();
//}

//Char Char::Parse(String const& s)
//{
//	//if (s.IsNullOrEmpty())  throw ArgumentNullException("s");
//
//
//	//if (s.Length() != 1) throw new FormatException(Environment.GetResourceString("Format_NeedSingleChar"));
//
//	//return s[0];
//	
//	return '\0';
//}
//
//Boolean Char::TryParse(String s, Char& result) noexcept
//{
//	result = '\0';
//
//	//if (s.IsNullOrEmpty())  return false;
//	//if (s.Length() != 1) return false; }
//	//result = s[0];
//
//	return true;
//}

Boolean Char::IsDigit(Char const& ch) noexcept
{
	//if (IsLatin1(ch))
	//{
	//	return (ch >= '0' && ch <= '9');
	//}

	// return (CharUnicodeInfo.GetUnicodeCategory(c) == UnicodeCategory.DecimalDigitNumber);

	return true;
}

Boolean Char::CheckLetter(const UnicodeCategory& uc) noexcept
{
	switch (uc)
	{
	case (UnicodeCategory::UppercaseLetter):
	case (UnicodeCategory::LowercaseLetter):
	case (UnicodeCategory::TitlecaseLetter):
	case (UnicodeCategory::ModifierLetter):
	case (UnicodeCategory::OtherLetter):
		return true;
	}

	return false;
}

Boolean Char::IsLetter(Char const& ch) noexcept
{
	/*if (IsLatin1(ch)) 
	{
		if (IsASCII(ch))
		{
			ch |= static_cast<char>(0x20);
			return ((c >= 'a' && c <= 'z'));
		}

		return (CheckLetter(GetLatin1UnicodeCategory(c)));
	}

	return (CheckLetter(CharUnicodeInfo.GetUnicodeCategory(c)));*/

	return true;
}

Boolean Char::IsWhiteSpace(Char const& ch) noexcept
{
	/*if (IsLatin1(ch)) {
		return (IsWhiteSpaceLatin1(ch));
	}
	return CharUnicodeInfo.IsWhiteSpace(c);*/
	return true;
}

Boolean Char::IsUpper(Char const& ch) noexcept
{
	/*if (IsLatin1(c)) {
		if (IsAscii(c)) {
			return (c >= 'A' && c <= 'Z');
		}
		return (GetLatin1UnicodeCategory(c) == UnicodeCategory.UppercaseLetter);
	}
	return (CharUnicodeInfo.GetUnicodeCategory(c) == UnicodeCategory.UppercaseLetter);*/

	return true;
}

Boolean Char::IsLower(Char const& ch) noexcept
{
	/*if (IsLatin1(c)) {
		if (IsAscii(c)) {
			return (c >= 'a' && c <= 'z');
		}
		return (GetLatin1UnicodeCategory(c) == UnicodeCategory.LowercaseLetter);
	}
	return (CharUnicodeInfo.GetUnicodeCategory(c) == UnicodeCategory.LowercaseLetter);*/

	return true;
}

Boolean Char::CheckPunctuation(const UnicodeCategory& uc) noexcept
{
	switch (uc)
	{
	case UnicodeCategory::ConnectorPunctuation:
	case UnicodeCategory::DashPunctuation:
	case UnicodeCategory::OpenPunctuation:
	case UnicodeCategory::ClosePunctuation:
	case UnicodeCategory::InitialQuotePunctuation:
	case UnicodeCategory::FinalQuotePunctuation:
	case UnicodeCategory::OtherPunctuation:
		return true;
	}

	return false;
}

Boolean Char::IsPunctuation(Char const& ch) noexcept
{
	/*if (IsLatin1(c)) 
	{
		return (CheckPunctuation(GetLatin1UnicodeCategory(c)));
	}

	return (CheckPunctuation(CharUnicodeInfo.GetUnicodeCategory(c)));*/

	return true;
}

Boolean Char::CheckLetterOrDigit(const UnicodeCategory& uc) noexcept
{
	switch (uc)
	{
	case UnicodeCategory::UppercaseLetter:
	case UnicodeCategory::LowercaseLetter:
	case UnicodeCategory::TitlecaseLetter:
	case UnicodeCategory::ModifierLetter:
	case UnicodeCategory::OtherLetter:
	case UnicodeCategory::DecimalDigitNumber:
		return true;
	}

	return false;
}

Boolean Char::IsLetterOrDigit(Char ch) noexcept
{
	/*if (IsLatin1(c)) {
		return (CheckLetterOrDigit(GetLatin1UnicodeCategory(c)));
	}
	return (CheckLetterOrDigit(CharUnicodeInfo.GetUnicodeCategory(c)));*/

	return true;
}

Char Char::ToUpper(Char const& ch) noexcept
{
	/*if (culture == null)
		throw new ArgumentNullException("culture");
	Contract.EndContractBlock();
	return culture.TextInfo.ToUpper(c);*/

	return '\0';
}

Char Char::ToLower(Char const& ch) noexcept
{
	/*if (culture == null)
		throw new ArgumentNullException("culture");
	Contract.EndContractBlock();
	return culture.TextInfo.ToLower(c);*/

	return '\0';
}