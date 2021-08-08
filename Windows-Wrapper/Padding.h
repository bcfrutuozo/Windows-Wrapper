#pragma once

#include "Base.h"
#include "IEquatable.h"
#include "Size.h"

struct Padding : public Base
{
public:

	int Bottom;
	int Left;
	int Right;
	int Top;

	Padding(int padding) noexcept;
	Padding(int left, int top, int right, int bottom) noexcept;

	Padding operator+(const Padding& p) noexcept;
	Padding operator-(const Padding& p) noexcept;
	bool operator==(const Padding& p) const noexcept;
	static Padding Add(Padding& p1, Padding& p2) noexcept;
	static Padding Subtract(Padding& p1, Padding& p2) noexcept;
	void SetAll(int padding) noexcept;
	int GetHorizontal() const noexcept;
	int GetVertical() const noexcept;
	Size GetSize() const noexcept;
};

