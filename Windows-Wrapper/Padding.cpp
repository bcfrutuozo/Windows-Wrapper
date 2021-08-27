#include "Padding.h"

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

bool Padding::operator==(const Padding& p) const noexcept
{
	return Left == p.Left && Top == p.Top && Right == p.Right && Bottom == p.Bottom;
}

bool Padding::Equals(const Padding& p) const noexcept
{
	return Left == p.Left && Top == p.Top && Right == p.Right && Bottom == p.Bottom;
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