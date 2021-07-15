#pragma once

#include "Common.h"

#include <algorithm>

#define saturate(x) (std::min)((std::max)(x, 0.0f), 1.0f)	// std::min and std::max between brackets to avoid default minmax macro call

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

namespace Math
{
	inline float Length(const XMFLOAT2& v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	inline float Length(const XMFLOAT3& v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline float Distance(const XMVECTOR& v1, const XMVECTOR& v2)
	{
		XMVECTOR vectorSub = XMVectorSubtract(v1, v2);
		XMVECTOR length = XMVector3Length(vectorSub);

		float distance = 0.0f;
		XMStoreFloat(&distance, length);
		return distance;
	}

	inline float DistanceSquared(const XMVECTOR& v1, const XMVECTOR& v2)
	{
		XMVECTOR vectorSub = XMVectorSubtract(v1, v2);
		XMVECTOR length = XMVector2LengthSq(vectorSub);

		float distance = 0.0f;
		XMStoreFloat(&distance, length);
		return distance;
	}

	inline float DistanceEstimated(const XMVECTOR& v1, const XMVECTOR& v2)
	{
		XMVECTOR vectorSub = XMVectorSubtract(v1, v2);
		XMVECTOR length = XMVector3LengthEst(vectorSub);

		float distance = 0.0f;
		XMStoreFloat(&distance, length);
		return distance;
	}

	inline float Distance(const XMFLOAT2& v1, const XMFLOAT2& v2)
	{
		XMVECTOR vector1 = XMLoadFloat2(&v1);
		XMVECTOR vector2 = XMLoadFloat2(&v2);
		return XMVectorGetX(XMVector2Length(vector2 - vector1));
	}

	inline float Distance(const XMFLOAT3& v1, const XMFLOAT3& v2)
	{
		XMVECTOR vector1 = XMLoadFloat3(&v1);
		XMVECTOR vector2 = XMLoadFloat3(&v2);
		return Distance(vector1, vector2);
	}

	inline float DistanceSquared(const XMFLOAT3& v1, const XMFLOAT3& v2)
	{
		XMVECTOR vector1 = XMLoadFloat3(&v1);
		XMVECTOR vector2 = XMLoadFloat3(&v2);
		return DistanceSquared(vector1, vector2);
	}

	inline float DistanceEstimated(const XMFLOAT3& v1, const XMFLOAT3& v2)
	{
		XMVECTOR vector1 = XMLoadFloat3(&v1);
		XMVECTOR vector2 = XMLoadFloat3(&v2);
		return DistanceEstimated(vector1, vector2);
	}

	inline XMVECTOR ClosestPointOnLine(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& point)
	{
		XMVECTOR ab = b - a;
		XMVECTOR t = XMVector3Dot(point - a, ab) / XMVector3Dot(ab, ab);
		return a + (t * ab);
	}

	inline XMVECTOR ClosestPointOnLineSegment(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& point)
	{
		XMVECTOR ab = b - a;
		XMVECTOR t = XMVector3Dot(point - a, ab) / XMVector3Dot(ab, ab);
		return a + XMVectorSaturate(t) * ab;
	}

	inline constexpr XMFLOAT3 GetVectorHalfwayPoint(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3((a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f);
	}

	inline constexpr float InverseLerp(float value1, float value2, float pos)
	{
		return (pos - value1) / (value2 - value1);
	}

	inline constexpr float Lerp(float value1, float value2, float amount)
	{
		return value1 + (value2 - value1) * amount;
	}

	inline constexpr XMFLOAT2 Lerp(const XMFLOAT2& a, const XMFLOAT2& b, float i)
	{
		return XMFLOAT2(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i));
	}

	inline constexpr XMFLOAT3 Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float i)
	{
		return XMFLOAT3(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i));
	}

	inline constexpr XMFLOAT4 Lerp(const XMFLOAT4& a, const XMFLOAT4& b, float i)
	{
		return XMFLOAT4(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i), Lerp(a.w, b.w, i));
	}

	inline XMFLOAT4 Slerp(const XMFLOAT4& a, const XMFLOAT4& b, float i)
	{
		XMVECTOR _a = XMLoadFloat4(&a);
		XMVECTOR _b = XMLoadFloat4(&b);
		XMVECTOR result = XMQuaternionSlerp(_a, _b, i);
		XMFLOAT4 retValue;
		XMStoreFloat4(&retValue, result);
		return retValue;
	}

	inline constexpr XMFLOAT3 Max(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));	// std::min and std::max between brackets to avoid default minmax macro call
	}

	inline constexpr XMFLOAT3 Min(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));	// std::min and std::max between brackets to avoid default minmax macro call
	}

	inline constexpr float Clamp(float val, float min, float max)
	{
		if (val < min)
		{
			return min;
		}
		else if (val > max)
		{
			return max;
		}

		return val;
	}

	inline constexpr float SmoothStep(float value1, float value2, float amount)
	{
		amount = Clamp((amount - value1) / (value2 - value1), 0.0f, 1.0f);
		return amount * amount * amount * (amount * (amount * 6 - 15) + 10);
	}

	inline constexpr bool Collision2D(const XMFLOAT2& hitBox1Pos, const XMFLOAT2& hitBox1Size, const XMFLOAT2& hitBox2Pos, const XMFLOAT2& hitBox2Size)
	{
		if (hitBox1Pos.x + hitBox1Size.x < hitBox2Pos.x)
		{
			return false;
		}
		else if (hitBox1Pos.x > hitBox2Pos.x + hitBox2Size.x)
		{
			return false;
		}
		else if (hitBox1Pos.y + hitBox1Size.y < hitBox2Pos.y)
		{
			return false;
		}
		else if (hitBox1Pos.y > hitBox2Pos.y + hitBox2Size.y)
		{
			return false;
		}

		return true;
	}

	inline constexpr uint32_t GetNextPowerOfTwo(uint32_t x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}

	float TriangleArea(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c);
	float TriangleArea(float a, float b, float c);
	XMFLOAT3 GetCubicHermiteSplinePos(const XMFLOAT3& startPos, const XMFLOAT3& endPos, const XMFLOAT3& startTangent, const XMFLOAT3& endTangent, float atInterval);
	XMFLOAT3 GetQuadraticBezierPos(const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c, float i);
	XMFLOAT3 GetQuadraticBezierPos(const XMFLOAT4& a, const XMFLOAT4& b, const XMFLOAT4& c, float i);
	XMFLOAT3 QuaternionToRollPitchYaw(const XMFLOAT4& quaternion);
	XMVECTOR GetClosestPointToLine(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& p, bool segmentClamp = false);
	float GetPointSegmentDistance(const XMVECTOR& point, const XMVECTOR& segmentA, const XMVECTOR& segmentB);
	float GetAngle(const XMFLOAT2& a, const XMFLOAT2& b);
	void ConstructTriangleEquilateral(float radius, XMFLOAT4& a, XMFLOAT4& b, XMFLOAT4& c);
	void GetBarycentric(const XMVECTOR& p, const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, float& u, float& v, float& w, bool clamp = false);

	// Returns an element of a precomputed halton sequence. Specify which iteration to get with index >= 0
	const XMFLOAT4& GetHaltonSequence(int index);

	uint32_t CompressNormal(const XMFLOAT3& normal);
	uint32_t CompressColor(const XMFLOAT3& color);
	uint32_t CompressColor(const XMFLOAT4& color);

	//-----------------------------------------------------------------------------
	// Compute the intersection of a ray (Origin, Direction) with a triangle 
	// (V0, V1, V2).  Return true if there is an intersection and also set *pDist 
	// to the distance along the ray to the intersection.
	// 
	// The algorithm is based on Moller, Tomas and Trumbore, "Fast, Minimum Storage 
	// Ray-Triangle Intersection", Journal of Graphics Tools, vol. 2, no. 1, 
	// pp 21-28, 1997.
	//
	//	Modified for CENgine to return barycentrics
	//-----------------------------------------------------------------------------
	_Use_decl_annotations_
		inline bool XM_CALLCONV RayTriangleIntersects(FXMVECTOR origin, FXMVECTOR direction, FXMVECTOR v0, GXMVECTOR v1, HXMVECTOR v2, float& distance, XMFLOAT2& bary)
	{
		const XMVECTOR g_RayEpsilon = XMVectorSet(1e-20f, 1e-20f, 1e-20f, 1e-20f);
		const XMVECTOR g_RayNegEpsilon = XMVectorSet(-1e-20f, -1e-20f, -1e-20f, -1e-20f);

		XMVECTOR zero = XMVectorZero();

		XMVECTOR e1 = XMVectorSubtract(v1, v0);
		XMVECTOR e2 = XMVectorSubtract(v2, v0);

		// p = Direction ^ e2
		XMVECTOR p = XMVector3Cross(direction, e2);

		// det = e1 * p
		XMVECTOR det = XMVector3Dot(e1, p);

		XMVECTOR u, v, t;
		if (XMVector3GreaterOrEqual(det, g_RayEpsilon))
		{
			// Determinate is positive (front side of the triangle)
			XMVECTOR s = XMVectorSubtract(origin, v0);

			// u = s * p
			u = XMVector3Dot(s, p);

			XMVECTOR NoIntersection = XMVectorLess(u, zero);
			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(u, det));

			// q = s ^ e1
			XMVECTOR q = XMVector3Cross(s, e1);

			// v = Direction * q
			v = XMVector3Dot(direction, q);

			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(v, zero));
			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(XMVectorAdd(u, v), det));

			// t = e2 * q
			t = XMVector3Dot(e2, q);

			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(t, zero));
			if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
			{
				distance = 0.f;
				return false;
			}
		}
		else if (XMVector3LessOrEqual(det, g_RayNegEpsilon))
		{
			// Determine is negative (back side of the triangle)
			XMVECTOR s = XMVectorSubtract(origin, v0);

			// u = s * p
			u = XMVector3Dot(s, p);

			XMVECTOR NoIntersection = XMVectorGreater(u, zero);
			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(u, det));

			// q = s ^ e1
			XMVECTOR q = XMVector3Cross(s, e1);

			// v = Direction * q
			v = XMVector3Dot(direction, q);

			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(v, zero));
			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(XMVectorAdd(u, v), det));

			// t = e2 * q
			t = XMVector3Dot(e2, q);

			NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(t, zero));
			if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
			{
				distance = 0.f;
				return false;
			}
		}
		else
		{
			// Parallel ray
			distance = 0.f;
			return false;
		}

		t = XMVectorDivide(t, det);

		const XMVECTOR invdet = XMVectorReciprocal(det);
		XMStoreFloat(&bary.x, u * invdet);
		XMStoreFloat(&bary.y, v * invdet);

		// Store the x-component to *pDistance
		XMStoreFloat(&distance, t);

		return true;
	}
}

template<typename T>
constexpr auto Square(const T& x) noexcept
{
	return x * x;
}

template<typename T>
T WrapAngle(T theta) noexcept
{
	constexpr T twoPi = (T)2 * (T)PI_D;
	const T mod = (T)fmod(theta, twoPi);

	if (mod > (T)PI_D)
	{
		return mod - twoPi;
	}
	else if (mod < -(T)PI_D)
	{
		return mod + twoPi;
	}

	return mod;
}

template<typename T>
constexpr T Interpolate(const T& source, const T& destination, float alpha) noexcept
{
	return source + (destination - source) * alpha;
}

template<typename T>
constexpr T ToRadian(T degree) noexcept
{
	return degree * PI / (T)(180.0);
}

template<typename T>
constexpr T Gauss(T x, T sigma) noexcept
{
	const auto ss = Square(sigma);
	return ((T)1.0 / Square((T)2.0 * (T)PI_D * ss)) * exp(-Square(x) / (T)2.0 * ss);
}