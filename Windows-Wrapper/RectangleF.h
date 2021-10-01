#pragma once

#include "PointF.h"
#include "SizeF.h"

struct Rectangle;

namespace Drawing
{
	struct RectangleF : public IEquatable<Drawing::RectangleF>
	{
	public:

		float X;
		float Y;
		float Width;
		float Height;

		constexpr RectangleF(float x, float y, float width, float height) noexcept : X(x), Y(y), Width(width), Height(height) {}
		constexpr RectangleF(PointF p, SizeF s) noexcept : X(p.X), Y(p.Y), Width(s.Width), Height(s.Height) {}

		constexpr bool operator==(RectangleF r) const noexcept { return (X == r.X && Y == r.Y && Width == r.Width && Height == r.Height); }
		constexpr bool operator!=(RectangleF r) const noexcept { return !(*this == r); }

		constexpr bool Contains(float x, float y) const noexcept { return (X <= x && x < (X + Width) && Y <= y && y < (Y + Height)); }
		constexpr bool Contains(PointF p) const noexcept { return Contains(p.X, p.Y); }
		constexpr bool Contains(RectangleF r) const noexcept { return (X <= r.X) && ((r.X + r.Width) <= (X + Width)) && (Y <= r.Y) && ((r.Y + r.Height) <= (Y + Height)); }
		constexpr bool IsEmpty() const noexcept { return (X == 0.0f && Y == 0.0f && Width == 0.0f && Height == 0.0f); }
		constexpr float GetLeft() const noexcept { return X; }
		constexpr float GetTop() const noexcept { return Y; }
		constexpr float GetRight() const noexcept { return Width - X; }
		constexpr float GetBottom() const noexcept { return Height - Y; }
		constexpr PointF GetLocation() const noexcept { return PointF(X, Y); }
		constexpr SizeF GetSize() const noexcept { return SizeF(Width, Height); }
		constexpr void Offset(float x, float y) noexcept { X += x; Y += y; }
		constexpr void Offset(PointF p) noexcept { return Offset(p.X, p.Y); }

		constexpr void Inflate(float width, float height) noexcept
		{
			X -= width;
			Y -= height;
			Width += 2 * width;
			Height += 2 * height;
		}

		constexpr void Inflate(SizeF s) noexcept { return Inflate(static_cast<float>(s.Width), static_cast<float>(s.Height)); }

		constexpr void Intersect(RectangleF rect) noexcept
		{
			RectangleF result = RectangleF::Intersect(rect, *this);

			X = result.X;
			Y = result.Y;
			Width = result.Width;
			Height = result.Height;
		}

		constexpr bool IntersectsWith(RectangleF r) noexcept { return (r.X < X + Width) && (X < (r.X + r.Width)) && (r.Y < Y + Height) && (Y < r.Y + r.Height); }

		static constexpr RectangleF FromLTRB(float left, float top, float right, float bottom) noexcept { return RectangleF(left, top, right - left, bottom - top); }

		static constexpr RectangleF Inflate(RectangleF r, float x, float y) noexcept
		{
			RectangleF rect = r;
			rect.Inflate(x, y);
			return rect;
		}

		static constexpr RectangleF Intersect(RectangleF lhs, RectangleF rhs) noexcept
		{
			float x1 = (std::max)(lhs.X, rhs.X);
			float x2 = (std::min)(lhs.X + lhs.Width, rhs.X + rhs.Width);
			float y1 = (std::max)(lhs.Y, rhs.Y);
			float y2 = (std::min)(lhs.Y + lhs.Height, rhs.Y + rhs.Height);

			if (x2 >= x1 && y2 >= y1)  return RectangleF(x1, y1, x2 - x1, y2 - y1);

			return RectangleF::Empty();
		}


		static constexpr RectangleF Union(RectangleF lhs, RectangleF rhs) noexcept
		{
			float x1 = (std::min)(lhs.X, rhs.X);
			float x2 = (std::max)(lhs.X + lhs.Width, rhs.X + rhs.Width);
			float y1 = (std::min)(lhs.Y, rhs.Y);
			float y2 = (std::max)(lhs.Y + lhs.Height, rhs.Y + rhs.Height);

			return RectangleF(x1, y1, x2 - x1, y2 - y1);
		}

		int GetHashCode() const override;
		bool Equals(const Object* const obj) const override;
		bool Equals(const RectangleF* const b) const override;
		const std::string ToString() const noexcept override;

		static constexpr RectangleF Empty() { return RectangleF(0, 0, 0, 0); }
	};
}