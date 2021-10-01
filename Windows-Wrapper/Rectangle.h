#pragma once

#include "Point.h"
#include "Size.h"
#include "RectangleF.h"

namespace Drawing
{
	struct Rectangle : public IEquatable<Drawing::Rectangle>
	{
	public:

		int X;
		int Y;
		int Width;
		int Height;

		constexpr Rectangle(int x, int y, int width, int height) noexcept : X(x), Y(y), Width(width), Height(height) {}
		constexpr Rectangle(Point p, Size s) noexcept : X(p.X), Y(p.Y), Width(s.Width), Height(s.Height) {}

		constexpr bool operator==(Rectangle r) const noexcept { return (X == r.X && Y == r.Y && Width == r.Width && Height == r.Height); }
		constexpr bool operator!=(Rectangle r) const noexcept { return !(*this == r); }

		constexpr bool Contains(int x, int y) const noexcept { return (X <= x && x < (X + Width) && Y <= y && y < (Y + Height)); }
		constexpr bool Contains(Point p) const noexcept { return Contains(p.X, p.Y); }
		constexpr bool Contains(Rectangle r) const noexcept { return (X <= r.X) && ((r.X + r.Width) <= (X + Width)) && (Y <= r.Y) && ((r.Y + r.Height) <= (Y + Height)); }
		constexpr bool IsEmpty() const noexcept { return (X == 0 && Y == 0 && Width == 0 && Height == 0); }
		constexpr int GetLeft() const noexcept { return X; }
		constexpr int GetTop() const noexcept { return Y; }
		constexpr int GetRight() const noexcept { return Width - X; }
		constexpr int GetBottom() const noexcept { return Height - Y; }
		constexpr Point GetLocation() const noexcept { return Point(X, Y); }
		constexpr Size GetSize() const noexcept { return Size(Width, Height); }
		constexpr void Offset(int x, int y) noexcept { X += x; Y += y; }
		constexpr void Offset(Point p) noexcept { return Offset(p.X, p.Y); }

		constexpr void Inflate(int width, int height) noexcept
		{
			X -= width;
			Y -= height;
			Width += 2 * width;
			Height += 2 * height;
		}

		constexpr void Inflate(Size s) noexcept { return Inflate(s.Width, s.Height); }

		constexpr void Intersect(Rectangle rect) noexcept
		{
			Rectangle result = Rectangle::Intersect(rect, *this);

			X = result.X;
			Y = result.Y;
			Width = result.Width;
			Height = result.Height;
		}

		constexpr bool IntersectsWith(Rectangle r) noexcept { return (r.X < X + Width) && (X < (r.X + r.Width)) && (r.Y < Y + Height) && (Y < r.Y + r.Height); }

		static constexpr Rectangle Inflate(Rectangle r, int x, int y) noexcept
		{
			Rectangle rect = r;
			rect.Inflate(x, y);
			return rect;
		}

		static constexpr Rectangle Intersect(Rectangle lhs, Rectangle rhs) noexcept
		{
			int x1 = (std::max)(lhs.X, rhs.X);
			int x2 = (std::min)(lhs.X + lhs.Width, rhs.X + rhs.Width);
			int y1 = (std::max)(lhs.Y, rhs.Y);
			int y2 = (std::min)(lhs.Y + lhs.Height, rhs.Y + rhs.Height);

			if (x2 >= x1 && y2 >= y1)  return Rectangle(x1, y1, x2 - x1, y2 - y1);

			return Rectangle::Empty();
		}


		static constexpr Rectangle Union(Rectangle lhs, Rectangle rhs) noexcept
		{
			int x1 = (std::min)(lhs.X, rhs.X);
			int x2 = (std::max)(lhs.X + lhs.Width, rhs.X + rhs.Width);
			int y1 = (std::min)(lhs.Y, rhs.Y);
			int y2 = (std::max)(lhs.Y + lhs.Height, rhs.Y + rhs.Height);

			return Rectangle(x1, y1, x2 - x1, y2 - y1);
		}

		static constexpr Rectangle Ceiling(RectangleF r) noexcept { return Rectangle(static_cast<int>(std::ceil(r.X)), static_cast<int>(std::ceil(r.Y)), static_cast<int>(std::ceil(r.Width)), static_cast<int>(std::ceil(r.Height))); }
		static constexpr Rectangle Truncate(RectangleF r) noexcept { return Rectangle(static_cast<int>(r.X), static_cast<int>(r.Y), static_cast<int>(r.Width), static_cast<int>(r.Height)); }
		static constexpr Rectangle Round(RectangleF r) noexcept { return Rectangle(static_cast<int>(std::round(r.X)), static_cast<int>(std::round(r.Y)), static_cast<int>(std::round(r.Width)), static_cast<int>(std::round(r.Height))); }

		int GetHashCode() const override;
		bool Equals(const Object* const obj) const override;
		bool Equals(const Rectangle* const b) const override;
		const std::string ToString() const noexcept override;

		static constexpr Rectangle Empty() { return Rectangle(0, 0, 0, 0); }
	};
}