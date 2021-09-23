#include "Padding.h"
#include "Size.h"
#include "Exceptions.h"

Padding::Padding(int padding) noexcept
	:
	Left(padding),
	Top(padding),
	Right(padding),
	Bottom(padding)
{

}

Padding::Padding(int left, int top, int right, int bottom) noexcept
	:
	Left(left),
	Top(top),
	Right(right),
	Bottom(bottom)
{

}

bool Padding::operator==(const Padding& p) const
{
	return Left == p.Left && Top == p.Top && Right == p.Right && Bottom == p.Bottom;
}

bool Padding::Equals(const Object* const p) const
{
	if (p == nullptr) return false;

	if (const auto pad = dynamic_cast<const Padding*>(p))
	{
		return *this == *pad;
	}

	throw ArgumentException("Arg_MustBePadding");
}

bool Padding::Equals(const Padding* const s) const
{
	if (s == nullptr) return false;
	return *this == *s;
}

Padding Padding::operator+(const Padding& p) noexcept
{
	return Padding(Left + Left, Top + p.Top, Right + p.Right, Bottom + p.Bottom);
}

Padding Padding::operator-(const Padding& p) noexcept
{
	return Padding(Left - p.Left, Top - p.Top, Right - p.Right, Bottom - p.Bottom);
}

Padding Padding::Add(Padding& p1, Padding& p2) noexcept
{
	return p1 + p2;
}

Padding Padding::Subtract(Padding& p1, Padding& p2) noexcept
{
	return p1 - p2;
}

void Padding::SetAll(int padding) noexcept
{
	Left = Top = Right = Bottom = padding;
}

int Padding::GetHorizontal() const noexcept
{
	return Left + Right;
}

int Padding::GetVertical() const noexcept
{
	return Top + Bottom;
}

Size Padding::GetSize() const noexcept
{
	return Size(GetHorizontal(), GetVertical());
}