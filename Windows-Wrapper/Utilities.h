#pragma once

#include "Common.h"
#include "Point.h"
#include "Size.h"
#include "PointF.h"
#include "SizeF.h"

namespace Utilities
{
	inline bool CaseInsensitiveCharCompareN(char a, char b);
	inline bool CaseInsensitiveCharCompareW(wchar_t a, wchar_t b);
	bool CaseInsensitiveCompare(const std::string& s1, const std::string& s2);
	bool CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2);
}

namespace std
{
	template<class _container, class _Ty>
	inline bool contains(_container _C, const _Ty& _Val)
	{
		return std::find(_C.begin(), _C.end(), _Val) != _C.end();
	}
};

constexpr Point operator+(Point const& lhs, Size const& rhs) noexcept
{
	return Point(lhs.X + rhs.Width, lhs.Y + rhs.Height); 
}

constexpr Point operator-(Point const& lhs, Size const& rhs) noexcept
{ 
	return Point(lhs.X - rhs.Width, lhs.Y - rhs.Height); 
}

constexpr PointF operator+(PointF const& lhs, Size  const& rhs) noexcept 
{
	return PointF(lhs.X + rhs.Width, lhs.Y + rhs.Height); }

constexpr PointF operator+(PointF const& lhs, SizeF const& rhs) noexcept 
{
	return PointF(lhs.X + rhs.Width, lhs.Y + rhs.Height); 
}

constexpr PointF operator-(PointF const& lhs, Size  const& rhs) noexcept
{
	return PointF(lhs.X - rhs.Width, lhs.Y - rhs.Height); 
}

constexpr PointF operator-(PointF const& lhs, SizeF const& rhs) noexcept 
{
	return PointF(lhs.X - rhs.Width, lhs.Y - rhs.Height);
}

constexpr SizeF operator*(Size const& lhs, SizeF const& rhs) noexcept 
{
	return SizeF(lhs.Width * rhs.Width, lhs.Height * rhs.Height);
}

constexpr SizeF operator*(Size const& lhs, float const& rhs) noexcept 
{ 
	return SizeF(lhs.Width * rhs, lhs.Height * rhs); 
}