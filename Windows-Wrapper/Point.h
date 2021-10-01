#pragma once

#include "IEquatable.h"

struct PointF;
struct Size;

struct Point : public IEquatable<Point>
{
public:

	int X;
	int Y;

	constexpr Point(int word) : X(LOWORD(word)), Y(HIWORD(word)) { }
	constexpr Point(int x, int y) : X(x), Y(y) { }
	Point(Size size);

	constexpr Point operator+(Point p) { return Point(X + p.X, Y + p.Y); }
	constexpr Point operator-(Point p) { return Point(X - p.X, Y - p.Y); }
	constexpr bool operator==(Point p) const { return (X == p.X && Y == p.Y); }

	constexpr bool IsEmpty() const noexcept { return (X == 0 && Y == 0); }

	inline Point Add(Point p, Size s) noexcept;
	inline Point CeilingF(PointF p) noexcept;
	inline Point Offset(int x, int y) noexcept;
	inline Point Offset(Point p) noexcept;
	inline Point RoundF(PointF p) noexcept;
	inline Point Subtract(Point p, Size s) noexcept;
	inline Point Truncate(PointF p) noexcept;

	static constexpr Point Add(Point lhs, Point rhs) noexcept { return Point(lhs.X + rhs.X, lhs.Y + rhs.Y); }
	static constexpr Point Subtract(Point lhs, Point rhs) noexcept { return Point(lhs.X - rhs.X, lhs.Y - rhs.Y); }

	int GetHashCode() const noexcept override;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Point* const p) const override;

	static constexpr Point Empty() { return Point(0, 0); }
};