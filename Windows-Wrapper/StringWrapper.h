//#pragma once
//
//#include "IEquatable.h"
//#include "IComparable.h"
//#include "IConvertible.h"
//
//#include "Boolean.h"
//
//class Char;
//
//enum class StringComparison
//{
//	CurrentCulture = 0,
//	CurrentCultureIgnoreCase = 1,
//	InvariantCulture = 2,
//	InvariantCultureIgnoreCase = 3,
//	Ordinal = 4,
//	OrdinalIgnoreCase = 5,
//};
//
//enum class StringSplitOptions
//{
//	None = 0,
//	RemoveEmptyEntries = 1
//};
//
//class String : public IComparable<String>, public IConvertible, public IEquatable<String>
//{
//private:
//
//	// Interface to provide access for generic string type.
//	// With it, the String class can handle simple char string (ASCII) and wide char string (UNICODE)
//	class IString
//	{
//	public:
//
//		virtual int GenerateHashCode() const = 0;
//		virtual constexpr bool IsEmpty() const noexcept = 0;
//		virtual constexpr size_t Length() const noexcept = 0;
//		virtual constexpr bool IsWhiteSpace() const noexcept = 0;
//		//virtual constexpr Char GetFirstChar() const noexcept = 0;
//	};
//
//	// Going to study UTF8 UTF16 and UTF32 encoding to handle all kinds of string manipulation
//	// For now, we're just focusing on char and widechar
//	template<typename T>
//	class StringBase : public IString
//	{
//	private:
//		std::basic_string<T, std::char_traits<T>, std::allocator<T>> base;
//	public:
//
//		constexpr StringBase(const T* c) { base = c; };
//		constexpr StringBase(std::basic_string<T, std::char_traits<T>, std::allocator<T>> cppString) { base = cppString; }
//
//		constexpr bool IsEmpty() const noexcept override { return base.empty(); }
//		constexpr size_t Length() const noexcept override { return base.length(); }
//		//constexpr Char GetFirstChar() const noexcept override { return &base.at(0); }
//
//		constexpr bool IsWhiteSpace() const noexcept override
//		{
//			if (base.find_first_not_of(' ') != std::basic_string<T, std::char_traits<T>, std::allocator<T>>::npos)
//			{
//				return false;
//			}
//
//			return true;
//		}
//
//		int GenerateHashCode() const override
//		{
//			assert(base[base.length()] == '\0', "src[this.Length] == '\\0'");
//			assert(((int)&base.at(0)) % 4 == 0, "Managed string should start at 4 bytes boundary");
//
//#if _WIN64
//			int hash1 = 5381;
//#else
//			int hash1 = (5381 << 16) + 5381;
//#endif
//			int hash2 = hash1;
//
//#if _WIN64
//			int c;
//			const T* s = &base.at(0);
//			while ((c = s[0]) != 0) {
//				hash1 = ((hash1 << 5) + hash1) ^ c;
//				c = s[1];
//				if (c == 0)
//					break;
//				hash2 = ((hash2 << 5) + hash2) ^ c;
//				s += 2;
//			}
//#else
//			int* pint = (int*)src;
//			int len = this.Length;
//			while (len > 2)
//			{
//				hash1 = ((hash1 << 5) + hash1 + (hash1 >> 27)) ^ pint[0];
//				hash2 = ((hash2 << 5) + hash2 + (hash2 >> 27)) ^ pint[1];
//				pint += 2;
//				len -= 4;
//			}
//
//			if (len > 0)
//			{
//				hash1 = ((hash1 << 5) + hash1 + (hash1 >> 27)) ^ pint[0];
//			}
//#endif			
//			return hash1 + (hash2 * 1566083941);
//		}
//	};
//
//	IString* m_value;
//	bool m_IsAscii;
//
//public:
//
//	// Empty or Null String constructor
//	constexpr String() : m_value(nullptr), m_IsAscii(false) {}
//
//	// ASCII or ISO-8859 character set
//	constexpr String(const char* c) : m_value(new StringBase<char>(c)), m_IsAscii(true) { }
//	constexpr String(const std::string& str) : m_value(new StringBase<char>(str)), m_IsAscii(true) { }
//
//	// UTF16 and windows default type
//	constexpr String(const wchar_t* c) : m_value(new StringBase<wchar_t>(c)), m_IsAscii(false) { }
//	constexpr String(const std::wstring& str) : m_value(new StringBase<wchar_t>(str)), m_IsAscii(false) { }
//
//	constexpr String(String const& other) noexcept
//	{
//		if (m_value != nullptr)
//		{
//			delete m_value;
//			m_value = nullptr;
//		}
//		
//		m_IsAscii = other.m_IsAscii;
//		m_value = other.m_value;
//	}
//
//	constexpr String(String&& other)
//	{
//		if (m_value != nullptr)
//		{
//			delete m_value;
//			m_value = nullptr;
//		}
//
//		m_IsAscii = other.m_IsAscii;
//		m_value = std::move(other.m_value);
//	}
//
//	constexpr String& operator=(String const& other)
//	{
//		if (m_value != nullptr)
//		{
//			delete m_value;
//			m_value = nullptr;
//		}
//
//		m_IsAscii = other.m_IsAscii;
//		m_value = other.m_value;
//		return *this;
//	}
//
//	constexpr String& operator=(String&& other)
//	{
//		if (m_value != nullptr)
//		{
//			delete m_value;
//			m_value = nullptr;
//		}
//
//		m_IsAscii = other.m_IsAscii;
//		m_value = std::move(other.m_value);
//		return *this;
//	}
//	
//	constexpr ~String() { if (m_value != nullptr) { delete m_value; m_value = nullptr; } }
//
//	constexpr size_t Length() const noexcept { return m_value == nullptr ? 0 : m_value->Length(); }
//	constexpr bool IsNullOrEmpty() const noexcept { return m_value == nullptr ? true : m_value->IsEmpty(); }
//	constexpr bool IsWhiteSpace() const noexcept { return m_value->IsWhiteSpace(); }
//
//	inline constexpr TypeCode GetTypeCode() const noexcept override { return TypeCode::String; }
//	inline int GetHashCode() const override;
//	inline int CompareTo(const Object* const obj) const override;
//	inline int CompareTo(const String* const value) const override;
//	inline bool Equals(const Object* const obj) const override;
//	inline bool Equals(const String* const value) const override;
//	//OVERRIDE OBJECT TO STRING inline const std::string ToString() const noexcept;
//	//inline String ToString() const noexcept override;
//	Boolean ToBoolean(IFormatProvider* provider) const override;
//	Int32 ToInt32(IFormatProvider* provider) const override;
//
//	static constexpr String Empty() { return String(); }
//};