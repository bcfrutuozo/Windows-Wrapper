#pragma once

enum class AnchorStyles
{
    Top = 0x01,
    Bottom = 0x02,
    Left = 0x04,
    Right = 0x08,
    None = 0,
};

constexpr inline AnchorStyles operator&(AnchorStyles a, AnchorStyles b)
{
    return static_cast<AnchorStyles>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr inline AnchorStyles operator|(AnchorStyles a, AnchorStyles b)
{
    return static_cast<AnchorStyles>(static_cast<int>(a) | static_cast<int>(b));
}