#pragma once

#include "SHA1.h"

#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <string_view>
#include <iterator>
#include <random>
#include <memory>
#include <functional>
#include <type_traits>
#include <assert.h>
#include <span>

#include <objbase.h>

template<typename TChar>
constexpr bool is_char_type_v = std::_Is_any_of_v<std::remove_cv_t<TChar>,
	char, wchar_t>;
template<typename T>
struct is_char_type : std::bool_constant <is_char_type_v<T>> {};


template <typename TChar, typename = std::enable_if<is_char_type<TChar>::value>>
constexpr inline unsigned char HexToChar(TChar const ch)
{
	if (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9')) return static_cast<unsigned char>(ch - static_cast<TChar>('0'));
	if (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f')) return static_cast<unsigned char>(10u + ch - static_cast<TChar>('a'));
	if (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F')) return static_cast<unsigned char>(10u + ch - static_cast<TChar>('A'));

	return 0u;
}

template <typename TChar, typename = std::enable_if<is_char_type<TChar>::value>>
constexpr inline bool IsHex(TChar const ch)
{
	return
		(ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9')) ||
		(ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f')) ||
		(ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'));
}

template <typename TChar, typename = std::enable_if<is_char_type<TChar>::value>>
constexpr inline unsigned char HexPairToChar(TChar const a, TChar const b)
{
	return (HexToChar(a) << 4) | HexToChar(b);
}

// UUID format https://tools.ietf.org/html/rfc4122
// Field	                     NDR Data Type	   Octet #	Note
// --------------------------------------------------------------------------------------------------------------------------
// time_low	                  unsigned long	   0 - 3	   The low field of the timestamp.
// time_mid	                  unsigned short	   4 - 5	   The middle field of the timestamp.
// time_hi_and_version	      unsigned short	   6 - 7	   The high field of the timestamp multiplexed with the version number.
// clock_seq_hi_and_reserved	unsigned small	   8	      The high field of the clock sequence multiplexed with the variant.
// clock_seq_low	            unsigned small	   9	      The low field of the clock sequence.
// node	                     character	      10 - 15	The spatially unique node identifier.
// --------------------------------------------------------------------------------------------------------------------------
// 0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                          time_low                             |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |       time_mid                |         time_hi_and_version   |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |clk_seq_hi_res |  clk_seq_low  |         node (0-1)            |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                         node (2-5)                            |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// indicated by a bit pattern in octet 8, marked with N in xxxxxxxx-xxxx-xxxx-Nxxx-xxxxxxxxxxxx
enum class uuid_variant
{
	// NCS backward compatibility (with the obsolete Apollo Network Computing System 1.5 UUID format)
	// N bit pattern: 0xxx
	// > the first 6 octets of the UUID are a 48-bit timestamp (the number of 4 microsecond units of time since 1 Jan 1980 UTC);
	// > the next 2 octets are reserved;
	// > the next octet is the "address family"; 
	// > the final 7 octets are a 56-bit host ID in the form specified by the address family
	ncs,

	// RFC 4122/DCE 1.1 
	// N bit pattern: 10xx
	// > big-endian byte order
	rfc,

	// Microsoft Corporation backward compatibility
	// N bit pattern: 110x
	// > little endian byte order
	// > formely used in the Component Object Model (COM) library      
	microsoft,

	// reserved for possible future definition
	// N bit pattern: 111x      
	reserved
};

struct uuid_error : public std::runtime_error
{
	explicit uuid_error(std::string_view message)
		: std::runtime_error(message.data())
	{
	}

	explicit uuid_error(char const* message)
		: std::runtime_error(message)
	{
	}
};

// indicated by a bit pattern in octet 6, marked with M in xxxxxxxx-xxxx-Mxxx-xxxx-xxxxxxxxxxxx
enum class uuid_version
{
	none = 0, // only possible for nil or invalid uuids
	time_based = 1,  // The time-based version specified in RFC 4122
	dce_security = 2,  // DCE Security version, with embedded POSIX UIDs.
	name_based_md5 = 3,  // The name-based version specified in RFS 4122 with MD5 hashing
	random_number_based = 4,  // The randomly or pseudo-randomly generated version specified in RFS 4122
	name_based_sha1 = 5   // The name-based version specified in RFS 4122 with SHA1 hashing
};

struct Guid
{
	struct uuid_const_iterator
	{
		using self_type = uuid_const_iterator;
		using value_type = uint8_t;
		using reference = uint8_t const&;
		using pointer = uint8_t const*;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = ptrdiff_t;

	protected:
		pointer ptr = nullptr;
		size_t  index = 0;

		bool compatible(self_type const& other) const noexcept
		{
			return ptr == other.ptr;
		}

	public:
		constexpr explicit uuid_const_iterator(pointer ptr, size_t const index) :
			ptr(ptr), index(index)
		{
		}

		uuid_const_iterator(uuid_const_iterator const& o) = default;
		uuid_const_iterator& operator=(uuid_const_iterator const& o) = default;
		~uuid_const_iterator() = default;

		self_type& operator++ ()
		{
			if (index >= 16)
				throw std::out_of_range("Iterator cannot be incremented past the end of the data.");
			++index;
			return *this;
		}

		self_type operator++ (int)
		{
			self_type tmp = *this;
			++* this;
			return tmp;
		}

		bool operator== (self_type const& other) const
		{
			assert(compatible(other));
			return index == other.index;
		}

		bool operator!= (self_type const& other) const
		{
			return !(*this == other);
		}

		reference operator* () const
		{
			if (ptr == nullptr)
				throw std::bad_function_call();
			return *(ptr + index);
		}

		reference operator-> () const
		{
			if (ptr == nullptr)
				throw std::bad_function_call();
			return *(ptr + index);
		}

		uuid_const_iterator() = default;

		self_type& operator--()
		{
			if (index <= 0)
				throw std::out_of_range("Iterator cannot be decremented past the beginning of the data.");
			--index;
			return *this;
		}

		self_type operator--(int)
		{
			self_type tmp = *this;
			--* this;
			return tmp;
		}

		self_type operator+(difference_type offset) const
		{
			self_type tmp = *this;
			return tmp += offset;
		}

		self_type operator-(difference_type offset) const
		{
			self_type tmp = *this;
			return tmp -= offset;
		}

		difference_type operator-(self_type const& other) const
		{
			assert(compatible(other));
			return (index - other.index);
		}

		bool operator<(self_type const& other) const
		{
			assert(compatible(other));
			return index < other.index;
		}

		bool operator>(self_type const& other) const
		{
			return other < *this;
		}

		bool operator<=(self_type const& other) const
		{
			return !(other < *this);
		}

		bool operator>=(self_type const& other) const
		{
			return !(*this < other);
		}

		self_type& operator+=(difference_type const offset)
		{
			if (static_cast<difference_type>(index) + offset < 0 ||
				static_cast<difference_type>(index) + offset > 16)
				throw std::out_of_range("Iterator cannot be incremented outside data bounds.");

			index += offset;
			return *this;
		}

		self_type& operator-=(difference_type const offset)
		{
			return *this += -offset;
		}

		value_type const& operator[](difference_type const offset) const
		{
			return (*(*this + offset));
		}
	};

	using value_type = uint8_t;

public:
	constexpr Guid() noexcept : data({}) {};

	explicit Guid(std::span<value_type, 16> bytes)
	{
		std::copy(std::cbegin(bytes), std::cend(bytes), std::begin(data));
	}

	template<typename ForwardIterator>
	explicit Guid(ForwardIterator first, ForwardIterator last)
	{
		if (std::distance(first, last) == 16)
			std::copy(first, last, std::begin(data));
	}

	constexpr uuid_variant variant() const noexcept
	{
		if ((data[8] & 0x80) == 0x00)
			return uuid_variant::ncs;
		else if ((data[8] & 0xC0) == 0x80)
			return uuid_variant::rfc;
		else if ((data[8] & 0xE0) == 0xC0)
			return uuid_variant::microsoft;
		else
			return uuid_variant::reserved;
	}

	constexpr uuid_version version() const noexcept
	{
		if ((data[6] & 0xF0) == 0x10)
			return uuid_version::time_based;
		else if ((data[6] & 0xF0) == 0x20)
			return uuid_version::dce_security;
		else if ((data[6] & 0xF0) == 0x30)
			return uuid_version::name_based_md5;
		else if ((data[6] & 0xF0) == 0x40)
			return uuid_version::random_number_based;
		else if ((data[6] & 0xF0) == 0x50)
			return uuid_version::name_based_sha1;
		else
			return uuid_version::none;
	}

	constexpr std::size_t size() const noexcept { return 16; }

	constexpr bool is_nil() const noexcept
	{
		for (size_t i = 0; i < data.size(); ++i) if (data[i] != 0) return false;
		return true;
	}

	void swap(Guid& other) noexcept
	{
		data.swap(other.data);
	}

	constexpr uuid_const_iterator begin() const noexcept { return uuid_const_iterator(&data[0], 0); }
	constexpr uuid_const_iterator end() const noexcept { return uuid_const_iterator(&data[0], 16); }

	inline std::span<std::byte const, 16> as_bytes() const
	{
		return std::span<std::byte const, 16>(reinterpret_cast<std::byte const*>(data.data()), 16);
	}

	template <typename TChar>
	static Guid from_string(TChar const* const str, size_t const size)
	{
		TChar digit = 0;
		bool firstDigit = true;
		int hasBraces = 0;
		size_t index = 0;
		std::array<uint8_t, 16> data{ { 0 } };

		if (str == nullptr || size == 0)
			throw uuid_error{ "Wrong uuid format" };

		if (str[0] == static_cast<TChar>('{'))
			hasBraces = 1;
		if (hasBraces && str[size - 1] != static_cast<TChar>('}'))
			throw uuid_error{ "Wrong uuid format" };

		for (size_t i = hasBraces; i < size - hasBraces; ++i)
		{
			if (str[i] == static_cast<TChar>('-')) continue;

			if (index >= 16 || !IsHex(str[i]))
			{
				throw uuid_error{ "Wrong uuid format" };
			}

			if (firstDigit)
			{
				digit = str[i];
				firstDigit = false;
			}
			else
			{
				data[index++] = HexPairToChar(digit, str[i]);
				firstDigit = true;
			}
		}

		if (index < 16)
		{
			throw uuid_error{ "Wrong uuid format" };
		}

		return Guid{ std::cbegin(data), std::cend(data) };
	}

	static Guid from_string(std::string_view str)
	{
		return from_string(str.data(), str.size());
	}

	static Guid from_string(std::wstring_view str)
	{
		return from_string(str.data(), str.size());
	}

private:
	std::array<value_type, 16> data{ { 0 } };

	friend bool operator==(Guid const& lhs, Guid const& rhs) noexcept;
	friend bool operator<(Guid const& lhs, Guid const& rhs) noexcept;

	template <class Elem, class Traits>
	friend std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& s, Guid const& id);
};

inline bool operator== (Guid const& lhs, Guid const& rhs) noexcept
{
	return lhs.data == rhs.data;
}

inline bool operator!= (Guid const& lhs, Guid const& rhs) noexcept
{
	return !(lhs == rhs);
}

inline bool operator< (Guid const& lhs, Guid const& rhs) noexcept
{
	return lhs.data < rhs.data;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& s, Guid const& id)
{
	return s << std::hex << std::setfill(static_cast<Elem>('0'))
		<< std::setw(2) << (int)id.data[0]
		<< std::setw(2) << (int)id.data[1]
		<< std::setw(2) << (int)id.data[2]
		<< std::setw(2) << (int)id.data[3]
		<< '-'
		<< std::setw(2) << (int)id.data[4]
		<< std::setw(2) << (int)id.data[5]
		<< '-'
		<< std::setw(2) << (int)id.data[6]
		<< std::setw(2) << (int)id.data[7]
		<< '-'
		<< std::setw(2) << (int)id.data[8]
		<< std::setw(2) << (int)id.data[9]
		<< '-'
		<< std::setw(2) << (int)id.data[10]
		<< std::setw(2) << (int)id.data[11]
		<< std::setw(2) << (int)id.data[12]
		<< std::setw(2) << (int)id.data[13]
		<< std::setw(2) << (int)id.data[14]
		<< std::setw(2) << (int)id.data[15];
}

inline std::string to_string(Guid const& id)
{
	std::stringstream sstr;
	sstr << id;
	return sstr.str();
}

inline std::wstring to_wstring(Guid const& id)
{
	std::wstringstream sstr;
	sstr << id;
	return sstr.str();
}

inline void swap(Guid& lhs, Guid& rhs)
{
	lhs.swap(rhs);
}

class uuid_system_generator
{
public:
	using result_type = Guid;

	Guid operator()()
	{
		// Remove compile warnings regarding RPC call
#pragma warning(push)
#pragma warning(disable : 6031)

		GUID newId;
		::CoCreateGuid(&newId);

#pragma warning(pop)

		std::array<uint8_t, 16> bytes =
		{ {
			  (unsigned char)((newId.Data1 >> 24) & 0xFF),
			  (unsigned char)((newId.Data1 >> 16) & 0xFF),
			  (unsigned char)((newId.Data1 >> 8) & 0xFF),
			  (unsigned char)((newId.Data1) & 0xFF),

			  (unsigned char)((newId.Data2 >> 8) & 0xFF),
			  (unsigned char)((newId.Data2) & 0xFF),

			  (unsigned char)((newId.Data3 >> 8) & 0xFF),
			  (unsigned char)((newId.Data3) & 0xFF),

			  newId.Data4[0],
			  newId.Data4[1],
			  newId.Data4[2],
			  newId.Data4[3],
			  newId.Data4[4],
			  newId.Data4[5],
			  newId.Data4[6],
			  newId.Data4[7]
		   } };

		return Guid{ std::begin(bytes), std::end(bytes) };
	}
};

template <typename UniformRandomNumberGenerator>
class basic_uuid_random_generator
{
public:
	using result_type = Guid;

	basic_uuid_random_generator()
		:generator(new UniformRandomNumberGenerator)
	{
		std::random_device rd;
		generator->seed(rd());
	}

	explicit basic_uuid_random_generator(UniformRandomNumberGenerator& gen) :
		generator(&gen, [](auto) {}) {}
	explicit basic_uuid_random_generator(UniformRandomNumberGenerator* gen) :
		generator(gen, [](auto) {}) {}

	Guid operator()()
	{
		uint8_t bytes[16] = { 0u };
		for (int i = 0; i < 16; i += 4)
			*reinterpret_cast<uint32_t*>(bytes + i) = distribution(*generator);

		// variant must be 10xxxxxx
		bytes[8] &= 0xBF;
		bytes[8] |= 0x80;

		// version must be 0100xxxx
		bytes[6] &= 0x4F;
		bytes[6] |= 0x40;

		return Guid{ std::begin(bytes), std::end(bytes) };
	}

private:
	std::uniform_int_distribution<uint32_t>  distribution;
	std::shared_ptr<UniformRandomNumberGenerator> generator;
};

using uuid_random_generator = basic_uuid_random_generator<std::mt19937>;

class uuid_name_generator
{
public:
	using result_type = Guid;

	explicit uuid_name_generator(Guid const& namespace_uuid) noexcept
		: nsuuid(namespace_uuid)
	{}

	Guid operator()(std::string_view name)
	{
		reset();
		process_characters(name.data(), name.size());
		return make_uuid();
	}

	Guid operator()(std::wstring_view name)
	{
		reset();
		process_characters(name.data(), name.size());
		return make_uuid();
	}

private:
	void reset()
	{
		hasher.Reset();
		uint8_t bytes[16];
		std::copy(std::begin(nsuuid), std::end(nsuuid), bytes);
		hasher.ProcessBytes(bytes, 16);
	}

	template <typename char_type,
		typename = std::enable_if_t<std::is_integral<char_type>::value>>
		void process_characters(char_type const* const characters, size_t const count)
	{
		for (size_t i = 0; i < count; i++)
		{
			uint32_t c = characters[i];
			hasher.ProcessByte(static_cast<unsigned char>((c >> 0) & 0xFF));
			hasher.ProcessByte(static_cast<unsigned char>((c >> 8) & 0xFF));
			hasher.ProcessByte(static_cast<unsigned char>((c >> 16) & 0xFF));
			hasher.ProcessByte(static_cast<unsigned char>((c >> 24) & 0xFF));
		}
	}

	void process_characters(const char* const characters, size_t const count)
	{
		hasher.ProcessBytes(characters, count);
	}

	Guid make_uuid()
	{
		SHA1::digest8_t digest;
		hasher.ComputeHash(digest);

		// variant must be 0b10xxxxxx
		digest[8] &= 0xBF;
		digest[8] |= 0x80;

		// version must be 0b0101xxxx
		digest[6] &= 0x5F;
		digest[6] |= 0x50;

		return Guid{ digest, digest + 16 };
	}

private:

	Guid nsuuid;
	SHA1 hasher;
};


namespace std
{
	template <>
	struct hash<Guid>
	{
		using argument_type = Guid;
		using result_type = std::size_t;

		result_type operator()(argument_type const& uuid) const
		{
			std::hash<std::string> hasher;
			return static_cast<result_type>(hasher(to_string(uuid)));
		}
	};
}
