#pragma once

#include "Object.h"
#include "IEquatable.h"
#include "Size.h"

struct Padding : public Object, public IEquatable<Padding>
{
public:

	int Bottom;
	int Left;
	int Right;
	int Top;

	Padding(int padding) noexcept;
	Padding(int left, int top, int right, int bottom) noexcept;

	bool operator==(const Padding& p) const noexcept override;
	bool Equals(const Padding& p) const noexcept override;

	inline Padding operator+(const Padding& p) noexcept;
	inline Padding operator-(const Padding& p) noexcept;
	inline static Padding Add(Padding& p1, Padding& p2) noexcept;
	inline static Padding Subtract(Padding& p1, Padding& p2) noexcept;
	void SetAll(int padding) noexcept;
	int GetHorizontal() const noexcept;
	int GetVertical() const noexcept;
	Size GetSize() const noexcept;
};

