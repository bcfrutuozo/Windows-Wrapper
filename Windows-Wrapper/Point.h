#pragma once

#include "IEquatable.h"

struct PointF;
struct Size;
struct SizeF;

struct Point : public IEquatable<Point>
{
public:

	int X;
	int Y;

	Point(int word);
	Point(int x, int y);
	Point(Size size);

	inline bool operator==(const Point& p) const;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Point* const p) const override;

	const bool IsEmpty() const noexcept;
	inline static Point Add(Point p, Size s) noexcept;
	inline static Point CeilingF(PointF p) noexcept;
	inline Point Offset(int x, int y) noexcept;
	inline Point Offset(Point p) noexcept;
	inline static Point RoundF(PointF p) noexcept;
	inline static Point Subtract(Point p, Size s) noexcept;
	inline static Point Truncate(PointF p) noexcept;
	inline Point operator+(Size s) noexcept;
	inline Point operator-(Size s) noexcept;
};

struct PointF : public IEquatable<PointF>
{
public:

	float X;
	float Y;

	PointF(float x, float y);

	inline bool operator==(const PointF& p) const;
	inline bool Equals(const Object* obj) const override;
	inline bool Equals(const PointF* const p) const override;

	const bool IsEmpty() const noexcept;
	inline static PointF Add(Point p, Size s) noexcept;
	inline static PointF Add(Point p, SizeF s) noexcept;
	inline static PointF Subtract(PointF p, Size s) noexcept;
	inline static PointF Subtract(PointF p, SizeF s) noexcept;
	inline PointF operator+(Size s) noexcept;
	inline PointF operator+(SizeF s) noexcept;
	inline PointF operator-(Size s) noexcept;
	inline PointF operator-(SizeF s) noexcept;
};