#pragma once

#include "IEquatable.h"
#include "Point.h"

struct SizeF;
struct Point;
struct PointF;

struct Size : public IEquatable<Size>
{
public:

	int Width;
	int Height;

	Size(int width, int height);
	Size(Point p);
	~Size() = default;

	inline bool Equals(Size s) const noexcept override;

	const bool IsEmpty() const noexcept;
	inline static Size Add(Size lhs, Size rhs) noexcept;
	inline static Size CeilingF(SizeF s) noexcept;
	inline static Size RoundF(SizeF s) noexcept;
	inline static Size Subtract(Size lhs, Size rhs) noexcept;
	inline static Size Truncate(SizeF s) noexcept;
	inline Size operator+(Size rhs) noexcept;
	inline Size operator/(int i);
	inline Size operator/(float f);
	inline bool operator==(Size s) noexcept;
	inline bool operator!=(Size s) noexcept;
	inline Size operator*(int i) noexcept;
	inline SizeF operator*(float f) noexcept;
	inline Size operator*(Size s) noexcept;
	inline SizeF operator*(SizeF s) noexcept;
	inline Size operator-(Size s) noexcept;
};

struct SizeF : public IEquatable<SizeF>
{
public:

	float Width;
	float Height;

	SizeF(float width, float height);
	SizeF(PointF p);
	virtual ~SizeF() = default;

	inline bool Equals(SizeF s) const noexcept override;

	const bool IsEmpty() const noexcept;
	inline static SizeF Add(SizeF lhs, SizeF rhs) noexcept;
	inline static SizeF Subtract(SizeF lhs, SizeF rhs) noexcept;
	inline PointF ToPointF() noexcept;
	inline Size ToSize() noexcept;
	inline SizeF operator+(SizeF rhs) noexcept;
	inline SizeF operator/(float f) noexcept;
	inline bool operator==(SizeF s) noexcept;
	inline bool operator!=(SizeF s) noexcept;
	inline SizeF operator*(float f) noexcept;
	inline SizeF operator*(SizeF f) noexcept;
	inline SizeF operator-(SizeF s) noexcept;
};
