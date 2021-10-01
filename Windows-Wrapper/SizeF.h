#pragma once

#include "IEquatable.h"

struct PointF;
struct Size;

struct SizeF : public IEquatable<SizeF>
{
public:

	float Width;
	float Height;

	constexpr SizeF(float width, float height) : Width(width), Height(height) { }
	SizeF(PointF p);

	constexpr SizeF operator+(SizeF s) noexcept { return SizeF(Width + s.Width, Height + s.Height); }
	constexpr SizeF operator-(SizeF s) noexcept { return SizeF(Width - s.Width, Height - s.Height); }
	constexpr SizeF operator/(float f) noexcept { return SizeF(Width * f, Height * f); }
	constexpr SizeF operator*(float f) noexcept { return SizeF(Width * f, Height * f); }
	constexpr SizeF operator*(SizeF s) noexcept { return SizeF(Width * s.Width, Height * s.Height); }
	constexpr bool operator==(SizeF s) const { return (Width == s.Width && Height == s.Height); }

	constexpr bool IsEmpty() const noexcept { return (Width == 0 && Height == 0); }

	inline PointF ToPointF() noexcept;
	inline Size ToSize() noexcept;

	static constexpr SizeF Add(SizeF lhs, SizeF rhs) noexcept { return SizeF(lhs.Width + rhs.Width, lhs.Height + rhs.Height); }
	static constexpr SizeF Subtract(SizeF lhs, SizeF rhs) noexcept { return SizeF(lhs.Width - rhs.Width, lhs.Height - rhs.Height); }

	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const SizeF* const s) const override;

	static constexpr SizeF Empty() { return SizeF(0.0f, 0.0f); }
};
