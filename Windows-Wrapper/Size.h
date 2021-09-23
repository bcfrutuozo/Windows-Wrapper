#pragma once

#include "IEquatable.h"

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

	inline bool operator==(const Size& s) const;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const Size* const s) const override;

	const bool IsEmpty() const noexcept;
	inline static Size Add(Size lhs, Size rhs) noexcept;
	inline static Size CeilingF(SizeF s) noexcept;
	inline static Size RoundF(SizeF s) noexcept;
	inline static Size Subtract(Size lhs, Size rhs) noexcept;
	inline static Size Truncate(SizeF s) noexcept;
	inline Size operator+(Size rhs) noexcept;
	inline Size operator/(int i);
	inline Size operator/(float f);
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

	inline bool operator==(const SizeF& s) const;
	inline bool Equals(const Object* const obj) const override;
	inline bool Equals(const SizeF* const s) const override;

	const bool IsEmpty() const noexcept;
	inline static SizeF Add(SizeF lhs, SizeF rhs) noexcept;
	inline static SizeF Subtract(SizeF lhs, SizeF rhs) noexcept;
	inline PointF ToPointF() noexcept;
	inline Size ToSize() noexcept;
	inline SizeF operator+(SizeF rhs) noexcept;
	inline SizeF operator/(float f) noexcept;
	inline SizeF operator*(float f) noexcept;
	inline SizeF operator*(SizeF f) noexcept;
	inline SizeF operator-(SizeF s) noexcept;
};
