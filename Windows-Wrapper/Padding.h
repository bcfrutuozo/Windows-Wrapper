#pragma once

#include "IEquatable.h"
#include "Size.h"

struct Padding : public IEquatable<Padding>
{
public:

	int Left;
	int Top;
	int Right;
	int Bottom;

	constexpr Padding(int padding)
		:
		Left(padding),
		Top(padding),
		Right(padding),
		Bottom(padding)
	{	}

	constexpr Padding(int left, int top, int right, int bottom)
		:
		Left(left),
		Top(top),
		Right(right),
		Bottom(bottom)
	{	}

	constexpr Padding operator+(const Padding& p) { return Padding(Left + Left, Top + p.Top, Right + p.Right, Bottom + p.Bottom); }
	constexpr Padding operator-(const Padding& p) { return Padding(Left - p.Left, Top - p.Top, Right - p.Right, Bottom - p.Bottom); }
	constexpr bool operator==(const Padding& p) const { return Left == p.Left && Top == p.Top && Right == p.Right && Bottom == p.Bottom; }

	constexpr Padding Add(Padding& p1, Padding& p2) { return p1 + p2; }
	constexpr Padding Subtract(Padding& p1, Padding& p2) { return p1 - p2; }
	constexpr void SetAll(int padding) { Left = Top = Right = Bottom = padding; }
	constexpr int GetHorizontal() const { return Left + Right; }
	constexpr int GetVertical() const { return Top + Bottom; }
	constexpr Size GetSize() const { return Size(GetHorizontal(), GetVertical()); }

	bool Equals(const Object* const p) const override;
	bool Equals(const Padding* const p) const override;

	static constexpr Padding Empty() { return Padding(0); }
};