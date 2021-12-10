#pragma once

enum class BoundsSpecified
{
    X = 0x1,
    Y = 0x2,
    Width = 0x4,
    Height = 0x8,
    Location = X | Y,
    Size = Width | Height,
    All = Location | Size,
    None = 0,
};

constexpr inline BoundsSpecified operator&(BoundsSpecified a, BoundsSpecified b)
{
    return static_cast<BoundsSpecified>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr inline BoundsSpecified operator|(BoundsSpecified a, BoundsSpecified b)
{
    return static_cast<BoundsSpecified>(static_cast<int>(a) | static_cast<int>(b));
}