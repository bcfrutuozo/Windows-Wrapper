#pragma once

#include "Point.h"
#include "Size.h"
#include "Padding.h"
#include "RectangleF.h"

#include <cmath>
#include <string>

namespace Drawing
{
	struct Rectangle : public IEquatable<Drawing::Rectangle>
	{
	public:

		int Left;
		int Top;
		int Right;
		int Bottom;

		constexpr Rectangle() noexcept : Left(0), Top(0), Right(0), Bottom(0) {}
		constexpr Rectangle(int x, int y, int width, int height) noexcept : Left(x), Top(y), Right(width), Bottom(height) {}
		constexpr Rectangle(Point p, Size s) noexcept : Left(p.X), Top(p.Y), Right(s.Width), Bottom(s.Height) {}

		constexpr bool operator==(Rectangle r) const noexcept { return (Left == r.Left && Top == r.Top && Right == r.Right && Bottom == r.Bottom); }
		constexpr bool operator!=(Rectangle r) const noexcept { return !(*this == r); }

		constexpr bool Contains(int x, int y) const noexcept { return (Left <= x && x < (Left + Right) && Top <= y && y < (Top + Bottom)); }
		constexpr bool Contains(Point p) const noexcept { return Contains(p.X, p.Y); }
		constexpr bool Contains(Rectangle r) const noexcept { return (Left <= r.Left) && ((r.Left + r.Right) <= (Left + Right)) && (Top <= r.Top) && ((r.Top + r.Bottom) <= (Top + Bottom)); }
		constexpr bool IsEmpty() const noexcept { return (Left == 0 && Top == 0 && Right == 0 && Bottom == 0); }
		constexpr int GetWidth() const noexcept { return Right - Left; }
		constexpr int GetHeight() const noexcept { return Bottom - Top; }
		constexpr Size GetSize() const noexcept { return Size(Right - Left, Bottom - Top); }
		constexpr void Offset(int x, int y) noexcept { Left += x; Top += y; }
		constexpr void Offset(Point p) noexcept { return Offset(p.X, p.Y); }

		constexpr void Inflate(int width, int height) noexcept
		{
			Left -= width;
			Top -= height;
			Right += width;
			Bottom += height;
		}

		constexpr void Deflate(int width, int height) noexcept
		{
			Left += width;
			Top += height;
			Right -= width;
			Bottom -= height;
		}

		constexpr void AddMargin(Padding m) noexcept
		{
			Left += m.Left;
			Top += m.Top;
			Right -= m.Right;
			Bottom -= m.Bottom;
		}

		constexpr void Inflate(Size s) noexcept { Inflate(s.Width, s.Height); }

		constexpr void Deflate(Size s) noexcept { Deflate(s.Width, s.Height); }

		constexpr void Intersect(Rectangle rect) noexcept
		{
			Rectangle result = Rectangle::Intersect(rect, *this);

			Left = result.Left;
			Top = result.Top;
			Right = result.Right;
			Bottom = result.Bottom;
		}

		constexpr bool IntersectsWith(Rectangle r) noexcept { return (r.Left < Left + Right) && (Left < (r.Left + r.Right)) && (r.Top < Top + Bottom) && (Top < r.Top + r.Bottom); }

		static constexpr Rectangle Inflate(Rectangle r, int x, int y) noexcept
		{
			Rectangle rect = r;
			rect.Inflate(x, y);
			return rect;
		}

		static constexpr Rectangle Deflate(Rectangle r, int x, int y) noexcept
		{
			Rectangle rect = r;
			rect.Deflate(x, y);
			return rect;
		}

		static constexpr Rectangle AddMargin(Rectangle r, Padding m) noexcept
		{
			Rectangle rect = r;
			rect.AddMargin(m);
			return rect;
		}

		static constexpr Rectangle Intersect(Rectangle lhs, Rectangle rhs) noexcept
		{
			int x1 = (std::max)(lhs.Left, rhs.Left);
			int x2 = (std::min)(lhs.Left + lhs.Right, rhs.Left + rhs.Right);
			int y1 = (std::max)(lhs.Top, rhs.Top);
			int y2 = (std::min)(lhs.Top + lhs.Bottom, rhs.Top + rhs.Bottom);

			if (x2 >= x1 && y2 >= y1)  return Rectangle(x1, y1, x2 - x1, y2 - y1);

			return Rectangle::Empty();
		}


		static constexpr Rectangle Union(Rectangle lhs, Rectangle rhs) noexcept
		{
			int x1 = (std::min)(lhs.Left, rhs.Left);
			int x2 = (std::max)(lhs.Left + lhs.Right, rhs.Left + rhs.Right);
			int y1 = (std::min)(lhs.Top, rhs.Top);
			int y2 = (std::max)(lhs.Top + lhs.Bottom, rhs.Top + rhs.Bottom);

			return Rectangle(x1, y1, x2 - x1, y2 - y1);
		}

		static constexpr Rectangle Ceiling(RectangleF r) noexcept { return Rectangle(static_cast<int>(std::ceil(r.Left)), static_cast<int>(std::ceil(r.Top)), static_cast<int>(std::ceil(r.Right)), static_cast<int>(std::ceil(r.Bottom))); }
		static constexpr Rectangle Truncate(RectangleF r) noexcept { return Rectangle(static_cast<int>(r.Left), static_cast<int>(r.Top), static_cast<int>(r.Right), static_cast<int>(r.Bottom)); }
		static constexpr Rectangle Round(RectangleF r) noexcept { return Rectangle(static_cast<int>(std::round(r.Left)), static_cast<int>(std::round(r.Top)), static_cast<int>(std::round(r.Right)), static_cast<int>(std::round(r.Bottom))); }

		int GetHashCode() const override;
		bool Equals(const Object* const obj) const override;
		bool Equals(const Rectangle* const b) const override;
		std::string ToString() const noexcept override;

		static constexpr Rectangle Empty() { return Rectangle(0, 0, 0, 0); }
	};
}