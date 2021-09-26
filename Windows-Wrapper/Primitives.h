#pragma once

#include "TypesInclude.h"

#include <iosfwd>

template<typename T, typename = std::enable_if_t< std::is_arithmetic<T>::value>>
class Primitive
{
protected:

    T m_value;

public:
    using value_type = T;

    constexpr Primitive() noexcept : m_value() {}

    template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
    constexpr Primitive(const U& value) noexcept : m_value(value) {}

    template<typename U>
    constexpr Primitive(Primitive<U> const& other) noexcept : m_value(other.Get()) {}

    constexpr T const& Get() const noexcept { return m_value; }

    template<typename U, typename = std::enable_if_t<std::is_fundamental<U>::value>>
    constexpr static Primitive From(U const& other) noexcept { return Primitive(U(other)); }

    constexpr explicit operator bool() const noexcept { return static_cast<bool>(m_value); }
    constexpr explicit operator char() const noexcept { return static_cast<char>(m_value); }
    constexpr explicit operator signed char() const noexcept { return static_cast<signed char>(m_value); }
    constexpr explicit operator unsigned char() const noexcept { return static_cast<unsigned char>(m_value); }
    constexpr explicit operator short() const noexcept { return static_cast<short>(m_value); }
    constexpr explicit operator int() const noexcept { return static_cast<int>(m_value); }
    constexpr explicit operator unsigned int() const noexcept { return static_cast<unsigned int>(m_value); }
    constexpr explicit operator long() const noexcept { return static_cast<long>(m_value); }
    constexpr explicit operator long long() const noexcept { return static_cast<long long>(m_value); }
    constexpr explicit operator float() const noexcept { return static_cast<float>(m_value); }
    constexpr explicit operator double() const noexcept { return static_cast<double>(m_value); }
    constexpr explicit operator long double() const noexcept { return static_cast<long double>(m_value); }
};