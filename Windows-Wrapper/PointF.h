#pragma once

#include "IEquatable.h"

struct Point;
struct Size;
struct SizeF;

struct PointF : public IEquatable<PointF>
{
public:

	float X;
	float Y;

	constexpr PointF(float x, float y) : X(x), Y(y) { }

	constexpr PointF operator+(PointF p) { return PointF(X + p.X, Y + p.Y); }
	constexpr PointF operator-(PointF p) { return PointF(X - p.X, Y - p.Y); }
	constexpr bool operator==(PointF p) const { return (X == p.X && Y == p.Y); }

	constexpr bool IsEmpty() const noexcept { return (X == 0 && Y == 0); }

	inline PointF Add(Point p, Size s) noexcept;
	inline PointF Add(Point p, SizeF s) noexcept;
	inline PointF Subtract(PointF p, Size s) noexcept;
	inline PointF Subtract(PointF p, SizeF s) noexcept;

	static constexpr PointF Add(PointF lhs, PointF rhs) noexcept { return PointF(lhs.X + rhs.X, lhs.Y + rhs.Y); }
	static constexpr PointF Subtract(PointF lhs, PointF rhs) noexcept { return PointF(lhs.X - rhs.X, lhs.Y - rhs.Y); }

	inline bool Equals(const Object* obj) const override;
	inline bool Equals(const PointF* const p) const override;

	static constexpr PointF Empty() { return PointF(0.0f, 0.0f); }
};