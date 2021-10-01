#pragma once

#include "IEquatable.h"

struct Point;
struct SizeF;

struct Size : public IEquatable<Size>
{
public:

	int Width;
	int Height;

	constexpr Size(int width, int height) : Width(width), Height(height) { }
	Size(Point p);

	constexpr Size operator+(Size s) noexcept { return Size(Width + s.Width, Height + s.Height); }
	constexpr Size operator-(Size s) noexcept { return Size(Width - s.Width, Height - s.Height); }
	constexpr Size operator*(Size s) noexcept { return Size(Width * s.Width, Height * s.Height); }
	constexpr bool operator==(Size s) const { return (Width == s.Width && Height == s.Height); }
	constexpr Size operator/(int i) { return Size(Width / i, Height / i); }
	constexpr Size operator*(int i) noexcept { return Size(Width * i, Height * i); }
	constexpr Size operator/(float f) { return Size(static_cast<int>(static_cast<float>(Width) / f), static_cast<int>(static_cast<float>(Height) / f)); }

	constexpr bool IsEmpty() const noexcept { return (Width == 0 && Height == 0); }

	inline Size CeilingF(SizeF s) noexcept;
	inline Size RoundF(SizeF s) noexcept;
	inline Size Truncate(SizeF s) noexcept;

	static constexpr Size Add(Size lhs, Size rhs) noexcept { return Size(lhs.Width + rhs.Width, lhs.Height + rhs.Height); }
	static constexpr Size Subtract(Size lhs, Size rhs) noexcept { return Size(lhs.Width - rhs.Width, lhs.Height - rhs.Height); }
	
	int GetHashCode() const noexcept override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Size* const s) const override;

	static constexpr Size Empty() { return Size(0, 0); }
};