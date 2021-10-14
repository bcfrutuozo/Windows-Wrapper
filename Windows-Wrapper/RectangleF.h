#pragma once

#include "IEquatable.h"
#include "PointF.h"
#include "SizeF.h"
#include "Padding.h"

#include <string>

struct Rectangle;

namespace Drawing
{
	struct RectangleF : public IEquatable<Drawing::RectangleF>
	{
	public:

		float Left;
		float Top;
		float Right;
		float Bottom;

		constexpr RectangleF() noexcept : Left(0.0f), Top(0.0f), Right(0.0f), Bottom(0.0f) {}
		constexpr RectangleF(float x, float y, float width, float height) noexcept : Left(x), Top(y), Right(width), Bottom(height) {}
		constexpr RectangleF(PointF p, SizeF s) noexcept : Left(p.X), Top(p.Y), Right(s.Width), Bottom(s.Height) {}

		constexpr bool operator==(RectangleF r) const noexcept { return (Left == r.Left && Top == r.Top && Right == r.Right && Bottom == r.Bottom); }
		constexpr bool operator!=(RectangleF r) const noexcept { return !(*this == r); }

		constexpr bool Contains(float x, float y) const noexcept { return (Left <= x && x < (Left + Right) && Top <= y && y < (Top + Bottom)); }
		constexpr bool Contains(PointF p) const noexcept { return Contains(p.X, p.Y); }
		constexpr bool Contains(RectangleF r) const noexcept { return (Left <= r.Left) && ((r.Left + r.Right) <= (Left + Right)) && (Top <= r.Top) && ((r.Top + r.Bottom) <= (Top + Bottom)); }
		constexpr bool IsEmpty() const noexcept { return (Left == 0.0f && Top == 0.0f && Right == 0.0f && Bottom == 0.0f); }
		constexpr float GetWidth() const noexcept { return Right - Left; }
		constexpr float GetHeight() const noexcept { return Bottom - Top; }
		constexpr SizeF GetSize() const noexcept { return SizeF(Right - Left, Bottom - Top); }
		constexpr void Offset(float x, float y) noexcept { Left += x; Top += y; }
		constexpr void Offset(PointF p) noexcept { return Offset(p.X, p.Y); }

		constexpr void Inflate(float width, float height) noexcept
		{
			Left -= width;
			Top -= height;
			Right += width;
			Bottom += height;
		}

		constexpr void Deflate(float width, float height) noexcept
		{
			Left += width;
			Top += height;
			Right -= width;
			Bottom -= height;
		}

		constexpr void AddMargin(Padding m) noexcept
		{
			Left += static_cast<float>(m.Left);
			Top += static_cast<float>(m.Top);
			Right -= static_cast<float>(m.Right);
			Bottom -= static_cast<float>(m.Bottom);
		}

		constexpr void Inflate(SizeF s) noexcept { Inflate(s.Width, s.Height); }

		constexpr void Deflate(SizeF s) noexcept { Deflate(s.Width, s.Height); }

		constexpr void Intersect(RectangleF rect) noexcept
		{
			RectangleF result = RectangleF::Intersect(rect, *this);

			Left = result.Left;
			Top = result.Top;
			Right = result.Right;
			Bottom = result.Bottom;
		}

		constexpr bool IntersectsWith(RectangleF r) noexcept { return (r.Left < Left + Right) && (Left < (r.Left + r.Right)) && (r.Top < Top + Bottom) && (Top < r.Top + r.Bottom); }

		static constexpr RectangleF FromLTRB(float left, float top, float right, float bottom) noexcept { return RectangleF(left, top, right - left, bottom - top); }

		static constexpr RectangleF Inflate(RectangleF r, float x, float y) noexcept
		{
			RectangleF rect = r;
			rect.Inflate(x, y);
			return rect;
		}

		static constexpr RectangleF Deflate(RectangleF r, float x, float y) noexcept
		{
			RectangleF rect = r;
			rect.Deflate(x, y);
			return rect;
		}

		static constexpr RectangleF AddMargin(RectangleF r, Padding m) noexcept
		{
			RectangleF rect = r;
			rect.AddMargin(m);
			return rect;
		}

		static constexpr RectangleF Intersect(RectangleF lhs, RectangleF rhs) noexcept
		{
			float x1 = (std::max)(lhs.Left, rhs.Left);
			float x2 = (std::min)(lhs.Left + lhs.Right, rhs.Left + rhs.Right);
			float y1 = (std::max)(lhs.Top, rhs.Top);
			float y2 = (std::min)(lhs.Top + lhs.Bottom, rhs.Top + rhs.Bottom);

			if (x2 >= x1 && y2 >= y1)  return RectangleF(x1, y1, x2 - x1, y2 - y1);

			return RectangleF::Empty();
		}


		static constexpr RectangleF Union(RectangleF lhs, RectangleF rhs) noexcept
		{
			float x1 = (std::min)(lhs.Left, rhs.Left);
			float x2 = (std::max)(lhs.Left + lhs.Right, rhs.Left + rhs.Right);
			float y1 = (std::min)(lhs.Top, rhs.Top);
			float y2 = (std::max)(lhs.Top + lhs.Bottom, rhs.Top + rhs.Bottom);

			return RectangleF(x1, y1, x2 - x1, y2 - y1);
		}

		int GetHashCode() const override;
		bool Equals(const Object* const obj) const override;
		bool Equals(const RectangleF* const b) const override;
		std::string ToString() const noexcept override;

		static constexpr RectangleF Empty() { return RectangleF(0, 0, 0, 0); }
	};
}