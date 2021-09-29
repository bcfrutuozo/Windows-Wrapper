#include "Mathlib.h"

namespace Math
{
	float TriangleArea(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c)
	{
		// Heron's formula:
		XMVECTOR a_ = XMVector3Length(b - a);
		XMVECTOR b_ = XMVector3Length(c - a);
		XMVECTOR c_ = XMVector3Length(c - b);
		XMVECTOR p = (a_ + b_ + c_) * 0.5f;
		XMVECTOR areaSq = p * (p - a_) * (p - b_) * (p - c_);
		float area;
		XMStoreFloat(&area, areaSq);
		area = sqrtf(area);
		return area;
	}

	float TriangleArea(float a, float b, float c)
	{
		// Heron's formula:
		float p = (a + b + c) * 0.5f;
		return sqrtf(p * (p - a) * (p - b) * (p - c));
	}

	XMFLOAT3 GetCubicHermiteSplinePos(const XMFLOAT3& startPos, const XMFLOAT3& endPos, const XMFLOAT3& startTangent, const XMFLOAT3& endTangent, float atInterval)
	{
		float x, y, z, t;

		t = atInterval;
		x = (2 * t * t * t - 3 * t * t + 1) * startPos.x + (-2 * t * t * t + 3 * t * t) * endPos.x + (t * t * t - 2 * t * t + t) * startTangent.x + (t * t * t - t * t) * endTangent.x;
		y = (2 * t * t * t - 3 * t * t + 1) * startPos.y + (-2 * t * t * t + 3 * t * t) * endPos.y + (t * t * t - 2 * t * t + 1) * startTangent.y + (t * t * t - t * t) * endTangent.y;
		z = (2 * t * t * t - 3 * t * t + 1) * startPos.z + (-2 * t * t * t + 3 * t * t) * endPos.z + (t * t * t - 2 * t * t + 1) * startTangent.z + (t * t * t - t * t) * endTangent.z;

		return XMFLOAT3(x, y, z);
	}

	XMFLOAT3 GetQuadraticBezierPos(const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c, float t)
	{
		float param0, param1, param2;

		param0 = powf(1 - t, 2);
		param1 = 2 * (1 - t) * t;
		param2 = powf(t, 2);
		float x = param0 * a.x + param1 * b.x + param2 * c.x;
		float y = param0 * a.y + param1 * b.y + param2 * c.y;
		float z = param0 * a.z + param1 * b.z + param2 * c.z;

		return XMFLOAT3(x, y, z);
	}

	XMFLOAT3 GetQuadraticBezierPos(const XMFLOAT4& a, const XMFLOAT4& b, const XMFLOAT4& c, float t)
	{
		return GetQuadraticBezierPos(XMFLOAT3(a.x, a.y, a.z), XMFLOAT3(b.x, b.y, b.z), XMFLOAT3(c.x, c.y, c.z), t);
	}

	XMFLOAT3 QuaternionToRollPitchYaw(const XMFLOAT4& quaternion)
	{
		float roll = atan2f(2 * quaternion.x * quaternion.w - 2 * quaternion.y * quaternion.z, 1 - 2 * quaternion.x * quaternion.x - 2 * quaternion.z * quaternion.z);
		float pitch = atan2f(2 * quaternion.y * quaternion.w - 2 * quaternion.x * quaternion.z, 1 - 2 * quaternion.y * quaternion.y - 2 * quaternion.z * quaternion.z);
		float yaw = asinf(2 * quaternion.x * quaternion.y + 2 * quaternion.z * quaternion.w);

		return XMFLOAT3(roll, pitch, yaw);
	}

	XMVECTOR GetClosestPointToLine(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& p, bool segmentClamp)
	{
		XMVECTOR ap_ = p - a;
		XMVECTOR ab_ = b - a;
		XMFLOAT3 ab, ap;
		XMStoreFloat3(&ab, ab_);
		XMStoreFloat3(&ap, ap_);
		float ab2 = ab.x * ab.x + ab.y * ab.y + ab.z * ab.z;
		float ap_ab = ap.x * ab.x + ap.y * ab.y + ap.z * ab.z;
		float t = ap_ab / ab2;
		if (segmentClamp)
		{
			if (t < 0.0f) t = 0.0f;
			else if (t > 1.0f) t = 1.0f;
		}
		XMVECTOR Closest = a + ab_ * t;
		return Closest;
	}
	float GetPointSegmentDistance(const XMVECTOR& point, const XMVECTOR& segmentA, const XMVECTOR& segmentB)
	{
		// Return minimum distance between line segment vw and point p
		const float l2 = XMVectorGetX(XMVector3LengthSq(segmentB - segmentA));  // i.e. |w-v|^2 -  avoid a sqrt
		if (l2 == 0.0) return Distance(point, segmentA);   // v == w case
															// Consider the line extending the segment, parameterized as v + t (w - v).
															// We find projection of point p onto the line. 
															// It falls where t = [(p-v) . (w-v)] / |w-v|^2
															// We clamp t from [0,1] to handle points outside the segment vw.
		const float t = (std::max)(0.0f, (std::min)(1.0f, XMVectorGetX(XMVector3Dot(point - segmentA, segmentB - segmentA)) / l2));		// std::min and std::max between brackets to avoid default minmax macro call
		const XMVECTOR projection = segmentA + t * (segmentB - segmentA);  // Projection falls on the segment
		return Distance(point, projection);
	}

	float GetAngle(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		float dot = a.x * b.x + a.y * b.y;  // dot product
		float det = a.x * b.y - a.y * b.x;	// determinant
		float angle = atan2f(det, dot);		// atan2(y, x) or atan2(sin, cos)
		if (angle < 0)
		{
			angle += XM_2PI;
		}
		return angle;
	}
	void ConstructTriangleEquilateral(float radius, XMFLOAT4& a, XMFLOAT4& b, XMFLOAT4& c)
	{
		float deg = 0;
		float t = deg * XM_PI / 180.0f;
		a = XMFLOAT4(radius * cosf(t), radius * -sinf(t), 0, 1);
		deg += 120;
		t = deg * XM_PI / 180.0f;
		b = XMFLOAT4(radius * cosf(t), radius * -sinf(t), 0, 1);
		deg += 120;
		t = deg * XM_PI / 180.0f;
		c = XMFLOAT4(radius * cosf(t), radius * -sinf(t), 0, 1);
	}
	void GetBarycentric(const XMVECTOR& p, const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR& c, float& u, float& v, float& w, bool clamp)
	{
		XMVECTOR v0 = b - a, v1 = c - a, v2 = p - a;
		float d00 = XMVectorGetX(XMVector3Dot(v0, v0));
		float d01 = XMVectorGetX(XMVector3Dot(v0, v1));
		float d11 = XMVectorGetX(XMVector3Dot(v1, v1));
		float d20 = XMVectorGetX(XMVector3Dot(v2, v0));
		float d21 = XMVectorGetX(XMVector3Dot(v2, v1));
		float denom = d00 * d11 - d01 * d01;
		v = (d11 * d20 - d01 * d21) / denom;
		w = (d00 * d21 - d01 * d20) / denom;
		u = 1.0f - v - w;

		if (clamp)
		{
			if (u < 0)
			{
				float t = XMVectorGetX(XMVector3Dot(p - b, c - b) / XMVector3Dot(c - b, c - b));
				t = Saturate(t);
				u = 0.0f;
				v = 1.0f - t;
				w = t;
			}
			else if (v < 0)
			{
				float t = XMVectorGetX(XMVector3Dot(p - c, a - c) / XMVector3Dot(a - c, a - c));
				t = Saturate(t);
				u = t;
				v = 0.0f;
				w = 1.0f - t;
			}
			else if (w < 0)
			{
				float t = XMVectorGetX(XMVector3Dot(p - a, b - a) / XMVector3Dot(b - a, b - a));
				t = Saturate(t);
				u = 1.0f - t;
				v = t;
				w = 0.0f;
			}
		}
	}

	const XMFLOAT4& GetHaltonSequence(int index)
	{
		static const XMFLOAT4 HALTON[] = {
			XMFLOAT4(0.5000000000f, 0.3333333333f, 0.2000000000f, 0.1428571429f),
			XMFLOAT4(0.2500000000f, 0.6666666667f, 0.4000000000f, 0.2857142857f),
			XMFLOAT4(0.7500000000f, 0.1111111111f, 0.6000000000f, 0.4285714286f),
			XMFLOAT4(0.1250000000f, 0.4444444444f, 0.8000000000f, 0.5714285714f),
			XMFLOAT4(0.6250000000f, 0.7777777778f, 0.0400000000f, 0.7142857143f),
			XMFLOAT4(0.3750000000f, 0.2222222222f, 0.2400000000f, 0.8571428571f),
			XMFLOAT4(0.8750000000f, 0.5555555556f, 0.4400000000f, 0.0204081633f),
			XMFLOAT4(0.0625000000f, 0.8888888889f, 0.6400000000f, 0.1632653061f),
			XMFLOAT4(0.5625000000f, 0.0370370370f, 0.8400000000f, 0.3061224490f),
			XMFLOAT4(0.3125000000f, 0.3703703704f, 0.0800000000f, 0.4489795918f),
			XMFLOAT4(0.8125000000f, 0.7037037037f, 0.2800000000f, 0.5918367347f),
			XMFLOAT4(0.1875000000f, 0.1481481481f, 0.4800000000f, 0.7346938776f),
			XMFLOAT4(0.6875000000f, 0.4814814815f, 0.6800000000f, 0.8775510204f),
			XMFLOAT4(0.4375000000f, 0.8148148148f, 0.8800000000f, 0.0408163265f),
			XMFLOAT4(0.9375000000f, 0.2592592593f, 0.1200000000f, 0.1836734694f),
			XMFLOAT4(0.0312500000f, 0.5925925926f, 0.3200000000f, 0.3265306122f),
			XMFLOAT4(0.5312500000f, 0.9259259259f, 0.5200000000f, 0.4693877551f),
			XMFLOAT4(0.2812500000f, 0.0740740741f, 0.7200000000f, 0.6122448980f),
			XMFLOAT4(0.7812500000f, 0.4074074074f, 0.9200000000f, 0.7551020408f),
			XMFLOAT4(0.1562500000f, 0.7407407407f, 0.1600000000f, 0.8979591837f),
			XMFLOAT4(0.6562500000f, 0.1851851852f, 0.3600000000f, 0.0612244898f),
			XMFLOAT4(0.4062500000f, 0.5185185185f, 0.5600000000f, 0.2040816327f),
			XMFLOAT4(0.9062500000f, 0.8518518519f, 0.7600000000f, 0.3469387755f),
			XMFLOAT4(0.0937500000f, 0.2962962963f, 0.9600000000f, 0.4897959184f),
			XMFLOAT4(0.5937500000f, 0.6296296296f, 0.0080000000f, 0.6326530612f),
			XMFLOAT4(0.3437500000f, 0.9629629630f, 0.2080000000f, 0.7755102041f),
			XMFLOAT4(0.8437500000f, 0.0123456790f, 0.4080000000f, 0.9183673469f),
			XMFLOAT4(0.2187500000f, 0.3456790123f, 0.6080000000f, 0.0816326531f),
			XMFLOAT4(0.7187500000f, 0.6790123457f, 0.8080000000f, 0.2244897959f),
			XMFLOAT4(0.4687500000f, 0.1234567901f, 0.0480000000f, 0.3673469388f),
			XMFLOAT4(0.9687500000f, 0.4567901235f, 0.2480000000f, 0.5102040816f),
			XMFLOAT4(0.0156250000f, 0.7901234568f, 0.4480000000f, 0.6530612245f),
			XMFLOAT4(0.5156250000f, 0.2345679012f, 0.6480000000f, 0.7959183673f),
			XMFLOAT4(0.2656250000f, 0.5679012346f, 0.8480000000f, 0.9387755102f),
			XMFLOAT4(0.7656250000f, 0.9012345679f, 0.0880000000f, 0.1020408163f),
			XMFLOAT4(0.1406250000f, 0.0493827160f, 0.2880000000f, 0.2448979592f),
			XMFLOAT4(0.6406250000f, 0.3827160494f, 0.4880000000f, 0.3877551020f),
			XMFLOAT4(0.3906250000f, 0.7160493827f, 0.6880000000f, 0.5306122449f),
			XMFLOAT4(0.8906250000f, 0.1604938272f, 0.8880000000f, 0.6734693878f),
			XMFLOAT4(0.0781250000f, 0.4938271605f, 0.1280000000f, 0.8163265306f),
			XMFLOAT4(0.5781250000f, 0.8271604938f, 0.3280000000f, 0.9591836735f),
			XMFLOAT4(0.3281250000f, 0.2716049383f, 0.5280000000f, 0.1224489796f),
			XMFLOAT4(0.8281250000f, 0.6049382716f, 0.7280000000f, 0.2653061224f),
			XMFLOAT4(0.2031250000f, 0.9382716049f, 0.9280000000f, 0.4081632653f),
			XMFLOAT4(0.7031250000f, 0.0864197531f, 0.1680000000f, 0.5510204082f),
			XMFLOAT4(0.4531250000f, 0.4197530864f, 0.3680000000f, 0.6938775510f),
			XMFLOAT4(0.9531250000f, 0.7530864198f, 0.5680000000f, 0.8367346939f),
			XMFLOAT4(0.0468750000f, 0.1975308642f, 0.7680000000f, 0.9795918367f),
			XMFLOAT4(0.5468750000f, 0.5308641975f, 0.9680000000f, 0.0029154519f),
			XMFLOAT4(0.2968750000f, 0.8641975309f, 0.0160000000f, 0.1457725948f),
			XMFLOAT4(0.7968750000f, 0.3086419753f, 0.2160000000f, 0.2886297376f),
			XMFLOAT4(0.1718750000f, 0.6419753086f, 0.4160000000f, 0.4314868805f),
			XMFLOAT4(0.6718750000f, 0.9753086420f, 0.6160000000f, 0.5743440233f),
			XMFLOAT4(0.4218750000f, 0.0246913580f, 0.8160000000f, 0.7172011662f),
			XMFLOAT4(0.9218750000f, 0.3580246914f, 0.0560000000f, 0.8600583090f),
			XMFLOAT4(0.1093750000f, 0.6913580247f, 0.2560000000f, 0.0233236152f),
			XMFLOAT4(0.6093750000f, 0.1358024691f, 0.4560000000f, 0.1661807580f),
			XMFLOAT4(0.3593750000f, 0.4691358025f, 0.6560000000f, 0.3090379009f),
			XMFLOAT4(0.8593750000f, 0.8024691358f, 0.8560000000f, 0.4518950437f),
			XMFLOAT4(0.2343750000f, 0.2469135802f, 0.0960000000f, 0.5947521866f),
			XMFLOAT4(0.7343750000f, 0.5802469136f, 0.2960000000f, 0.7376093294f),
			XMFLOAT4(0.4843750000f, 0.9135802469f, 0.4960000000f, 0.8804664723f),
			XMFLOAT4(0.9843750000f, 0.0617283951f, 0.6960000000f, 0.0437317784f),
			XMFLOAT4(0.0078125000f, 0.3950617284f, 0.8960000000f, 0.1865889213f),
			XMFLOAT4(0.5078125000f, 0.7283950617f, 0.1360000000f, 0.3294460641f),
			XMFLOAT4(0.2578125000f, 0.1728395062f, 0.3360000000f, 0.4723032070f),
			XMFLOAT4(0.7578125000f, 0.5061728395f, 0.5360000000f, 0.6151603499f),
			XMFLOAT4(0.1328125000f, 0.8395061728f, 0.7360000000f, 0.7580174927f),
			XMFLOAT4(0.6328125000f, 0.2839506173f, 0.9360000000f, 0.9008746356f),
			XMFLOAT4(0.3828125000f, 0.6172839506f, 0.1760000000f, 0.0641399417f),
			XMFLOAT4(0.8828125000f, 0.9506172840f, 0.3760000000f, 0.2069970845f),
			XMFLOAT4(0.0703125000f, 0.0987654321f, 0.5760000000f, 0.3498542274f),
			XMFLOAT4(0.5703125000f, 0.4320987654f, 0.7760000000f, 0.4927113703f),
			XMFLOAT4(0.3203125000f, 0.7654320988f, 0.9760000000f, 0.6355685131f),
			XMFLOAT4(0.8203125000f, 0.2098765432f, 0.0240000000f, 0.7784256560f),
			XMFLOAT4(0.1953125000f, 0.5432098765f, 0.2240000000f, 0.9212827988f),
			XMFLOAT4(0.6953125000f, 0.8765432099f, 0.4240000000f, 0.0845481050f),
			XMFLOAT4(0.4453125000f, 0.3209876543f, 0.6240000000f, 0.2274052478f),
			XMFLOAT4(0.9453125000f, 0.6543209877f, 0.8240000000f, 0.3702623907f),
			XMFLOAT4(0.0390625000f, 0.9876543210f, 0.0640000000f, 0.5131195335f),
			XMFLOAT4(0.5390625000f, 0.0041152263f, 0.2640000000f, 0.6559766764f),
			XMFLOAT4(0.2890625000f, 0.3374485597f, 0.4640000000f, 0.7988338192f),
			XMFLOAT4(0.7890625000f, 0.6707818930f, 0.6640000000f, 0.9416909621f),
			XMFLOAT4(0.1640625000f, 0.1152263374f, 0.8640000000f, 0.1049562682f),
			XMFLOAT4(0.6640625000f, 0.4485596708f, 0.1040000000f, 0.2478134111f),
			XMFLOAT4(0.4140625000f, 0.7818930041f, 0.3040000000f, 0.3906705539f),
			XMFLOAT4(0.9140625000f, 0.2263374486f, 0.5040000000f, 0.5335276968f),
			XMFLOAT4(0.1015625000f, 0.5596707819f, 0.7040000000f, 0.6763848397f),
			XMFLOAT4(0.6015625000f, 0.8930041152f, 0.9040000000f, 0.8192419825f),
			XMFLOAT4(0.3515625000f, 0.0411522634f, 0.1440000000f, 0.9620991254f),
			XMFLOAT4(0.8515625000f, 0.3744855967f, 0.3440000000f, 0.1253644315f),
			XMFLOAT4(0.2265625000f, 0.7078189300f, 0.5440000000f, 0.2682215743f),
			XMFLOAT4(0.7265625000f, 0.1522633745f, 0.7440000000f, 0.4110787172f),
			XMFLOAT4(0.4765625000f, 0.4855967078f, 0.9440000000f, 0.5539358601f),
			XMFLOAT4(0.9765625000f, 0.8189300412f, 0.1840000000f, 0.6967930029f),
			XMFLOAT4(0.0234375000f, 0.2633744856f, 0.3840000000f, 0.8396501458f),
			XMFLOAT4(0.5234375000f, 0.5967078189f, 0.5840000000f, 0.9825072886f),
			XMFLOAT4(0.2734375000f, 0.9300411523f, 0.7840000000f, 0.0058309038f),
			XMFLOAT4(0.7734375000f, 0.0781893004f, 0.9840000000f, 0.1486880466f),
			XMFLOAT4(0.1484375000f, 0.4115226337f, 0.0320000000f, 0.2915451895f),
			XMFLOAT4(0.6484375000f, 0.7448559671f, 0.2320000000f, 0.4344023324f),
			XMFLOAT4(0.3984375000f, 0.1893004115f, 0.4320000000f, 0.5772594752f),
			XMFLOAT4(0.8984375000f, 0.5226337449f, 0.6320000000f, 0.7201166181f),
			XMFLOAT4(0.0859375000f, 0.8559670782f, 0.8320000000f, 0.8629737609f),
			XMFLOAT4(0.5859375000f, 0.3004115226f, 0.0720000000f, 0.0262390671f),
			XMFLOAT4(0.3359375000f, 0.6337448560f, 0.2720000000f, 0.1690962099f),
			XMFLOAT4(0.8359375000f, 0.9670781893f, 0.4720000000f, 0.3119533528f),
			XMFLOAT4(0.2109375000f, 0.0164609053f, 0.6720000000f, 0.4548104956f),
			XMFLOAT4(0.7109375000f, 0.3497942387f, 0.8720000000f, 0.5976676385f),
			XMFLOAT4(0.4609375000f, 0.6831275720f, 0.1120000000f, 0.7405247813f),
			XMFLOAT4(0.9609375000f, 0.1275720165f, 0.3120000000f, 0.8833819242f),
			XMFLOAT4(0.0546875000f, 0.4609053498f, 0.5120000000f, 0.0466472303f),
			XMFLOAT4(0.5546875000f, 0.7942386831f, 0.7120000000f, 0.1895043732f),
			XMFLOAT4(0.3046875000f, 0.2386831276f, 0.9120000000f, 0.3323615160f),
			XMFLOAT4(0.8046875000f, 0.5720164609f, 0.1520000000f, 0.4752186589f),
			XMFLOAT4(0.1796875000f, 0.9053497942f, 0.3520000000f, 0.6180758017f),
			XMFLOAT4(0.6796875000f, 0.0534979424f, 0.5520000000f, 0.7609329446f),
			XMFLOAT4(0.4296875000f, 0.3868312757f, 0.7520000000f, 0.9037900875f),
			XMFLOAT4(0.9296875000f, 0.7201646091f, 0.9520000000f, 0.0670553936f),
			XMFLOAT4(0.1171875000f, 0.1646090535f, 0.1920000000f, 0.2099125364f),
			XMFLOAT4(0.6171875000f, 0.4979423868f, 0.3920000000f, 0.3527696793f),
			XMFLOAT4(0.3671875000f, 0.8312757202f, 0.5920000000f, 0.4956268222f),
			XMFLOAT4(0.8671875000f, 0.2757201646f, 0.7920000000f, 0.6384839650f),
			XMFLOAT4(0.2421875000f, 0.6090534979f, 0.9920000000f, 0.7813411079f),
			XMFLOAT4(0.7421875000f, 0.9423868313f, 0.0016000000f, 0.9241982507f),
			XMFLOAT4(0.4921875000f, 0.0905349794f, 0.2016000000f, 0.0874635569f),
			XMFLOAT4(0.9921875000f, 0.4238683128f, 0.4016000000f, 0.2303206997f),
			XMFLOAT4(0.0039062500f, 0.7572016461f, 0.6016000000f, 0.3731778426f),
			XMFLOAT4(0.5039062500f, 0.2016460905f, 0.8016000000f, 0.5160349854f),
			XMFLOAT4(0.2539062500f, 0.5349794239f, 0.0416000000f, 0.6588921283f),
			XMFLOAT4(0.7539062500f, 0.8683127572f, 0.2416000000f, 0.8017492711f),
			XMFLOAT4(0.1289062500f, 0.3127572016f, 0.4416000000f, 0.9446064140f),
			XMFLOAT4(0.6289062500f, 0.6460905350f, 0.6416000000f, 0.1078717201f),
			XMFLOAT4(0.3789062500f, 0.9794238683f, 0.8416000000f, 0.2507288630f),
			XMFLOAT4(0.8789062500f, 0.0288065844f, 0.0816000000f, 0.3935860058f),
			XMFLOAT4(0.0664062500f, 0.3621399177f, 0.2816000000f, 0.5364431487f),
			XMFLOAT4(0.5664062500f, 0.6954732510f, 0.4816000000f, 0.6793002915f),
			XMFLOAT4(0.3164062500f, 0.1399176955f, 0.6816000000f, 0.8221574344f),
			XMFLOAT4(0.8164062500f, 0.4732510288f, 0.8816000000f, 0.9650145773f),
			XMFLOAT4(0.1914062500f, 0.8065843621f, 0.1216000000f, 0.1282798834f),
			XMFLOAT4(0.6914062500f, 0.2510288066f, 0.3216000000f, 0.2711370262f),
			XMFLOAT4(0.4414062500f, 0.5843621399f, 0.5216000000f, 0.4139941691f),
			XMFLOAT4(0.9414062500f, 0.9176954733f, 0.7216000000f, 0.5568513120f),
			XMFLOAT4(0.0351562500f, 0.0658436214f, 0.9216000000f, 0.6997084548f),
			XMFLOAT4(0.5351562500f, 0.3991769547f, 0.1616000000f, 0.8425655977f),
			XMFLOAT4(0.2851562500f, 0.7325102881f, 0.3616000000f, 0.9854227405f),
			XMFLOAT4(0.7851562500f, 0.1769547325f, 0.5616000000f, 0.0087463557f),
			XMFLOAT4(0.1601562500f, 0.5102880658f, 0.7616000000f, 0.1516034985f),
			XMFLOAT4(0.6601562500f, 0.8436213992f, 0.9616000000f, 0.2944606414f),
			XMFLOAT4(0.4101562500f, 0.2880658436f, 0.0096000000f, 0.4373177843f),
			XMFLOAT4(0.9101562500f, 0.6213991770f, 0.2096000000f, 0.5801749271f),
			XMFLOAT4(0.0976562500f, 0.9547325103f, 0.4096000000f, 0.7230320700f),
			XMFLOAT4(0.5976562500f, 0.1028806584f, 0.6096000000f, 0.8658892128f),
			XMFLOAT4(0.3476562500f, 0.4362139918f, 0.8096000000f, 0.0291545190f),
			XMFLOAT4(0.8476562500f, 0.7695473251f, 0.0496000000f, 0.1720116618f),
			XMFLOAT4(0.2226562500f, 0.2139917695f, 0.2496000000f, 0.3148688047f),
			XMFLOAT4(0.7226562500f, 0.5473251029f, 0.4496000000f, 0.4577259475f),
			XMFLOAT4(0.4726562500f, 0.8806584362f, 0.6496000000f, 0.6005830904f),
			XMFLOAT4(0.9726562500f, 0.3251028807f, 0.8496000000f, 0.7434402332f),
			XMFLOAT4(0.0195312500f, 0.6584362140f, 0.0896000000f, 0.8862973761f),
			XMFLOAT4(0.5195312500f, 0.9917695473f, 0.2896000000f, 0.0495626822f),
			XMFLOAT4(0.2695312500f, 0.0082304527f, 0.4896000000f, 0.1924198251f),
			XMFLOAT4(0.7695312500f, 0.3415637860f, 0.6896000000f, 0.3352769679f),
			XMFLOAT4(0.1445312500f, 0.6748971193f, 0.8896000000f, 0.4781341108f),
			XMFLOAT4(0.6445312500f, 0.1193415638f, 0.1296000000f, 0.6209912536f),
			XMFLOAT4(0.3945312500f, 0.4526748971f, 0.3296000000f, 0.7638483965f),
			XMFLOAT4(0.8945312500f, 0.7860082305f, 0.5296000000f, 0.9067055394f),
			XMFLOAT4(0.0820312500f, 0.2304526749f, 0.7296000000f, 0.0699708455f),
			XMFLOAT4(0.5820312500f, 0.5637860082f, 0.9296000000f, 0.2128279883f),
			XMFLOAT4(0.3320312500f, 0.8971193416f, 0.1696000000f, 0.3556851312f),
			XMFLOAT4(0.8320312500f, 0.0452674897f, 0.3696000000f, 0.4985422741f),
			XMFLOAT4(0.2070312500f, 0.3786008230f, 0.5696000000f, 0.6413994169f),
			XMFLOAT4(0.7070312500f, 0.7119341564f, 0.7696000000f, 0.7842565598f),
			XMFLOAT4(0.4570312500f, 0.1563786008f, 0.9696000000f, 0.9271137026f),
			XMFLOAT4(0.9570312500f, 0.4897119342f, 0.0176000000f, 0.0903790087f),
			XMFLOAT4(0.0507812500f, 0.8230452675f, 0.2176000000f, 0.2332361516f),
			XMFLOAT4(0.5507812500f, 0.2674897119f, 0.4176000000f, 0.3760932945f),
			XMFLOAT4(0.3007812500f, 0.6008230453f, 0.6176000000f, 0.5189504373f),
			XMFLOAT4(0.8007812500f, 0.9341563786f, 0.8176000000f, 0.6618075802f),
			XMFLOAT4(0.1757812500f, 0.0823045267f, 0.0576000000f, 0.8046647230f),
			XMFLOAT4(0.6757812500f, 0.4156378601f, 0.2576000000f, 0.9475218659f),
			XMFLOAT4(0.4257812500f, 0.7489711934f, 0.4576000000f, 0.1107871720f),
			XMFLOAT4(0.9257812500f, 0.1934156379f, 0.6576000000f, 0.2536443149f),
			XMFLOAT4(0.1132812500f, 0.5267489712f, 0.8576000000f, 0.3965014577f),
			XMFLOAT4(0.6132812500f, 0.8600823045f, 0.0976000000f, 0.5393586006f),
			XMFLOAT4(0.3632812500f, 0.3045267490f, 0.2976000000f, 0.6822157434f),
			XMFLOAT4(0.8632812500f, 0.6378600823f, 0.4976000000f, 0.8250728863f),
			XMFLOAT4(0.2382812500f, 0.9711934156f, 0.6976000000f, 0.9679300292f),
			XMFLOAT4(0.7382812500f, 0.0205761317f, 0.8976000000f, 0.1311953353f),
			XMFLOAT4(0.4882812500f, 0.3539094650f, 0.1376000000f, 0.2740524781f),
			XMFLOAT4(0.9882812500f, 0.6872427984f, 0.3376000000f, 0.4169096210f),
			XMFLOAT4(0.0117187500f, 0.1316872428f, 0.5376000000f, 0.5597667638f),
			XMFLOAT4(0.5117187500f, 0.4650205761f, 0.7376000000f, 0.7026239067f),
			XMFLOAT4(0.2617187500f, 0.7983539095f, 0.9376000000f, 0.8454810496f),
			XMFLOAT4(0.7617187500f, 0.2427983539f, 0.1776000000f, 0.9883381924f),
			XMFLOAT4(0.1367187500f, 0.5761316872f, 0.3776000000f, 0.0116618076f),
			XMFLOAT4(0.6367187500f, 0.9094650206f, 0.5776000000f, 0.1545189504f),
			XMFLOAT4(0.3867187500f, 0.0576131687f, 0.7776000000f, 0.2973760933f),
			XMFLOAT4(0.8867187500f, 0.3909465021f, 0.9776000000f, 0.4402332362f),
			XMFLOAT4(0.0742187500f, 0.7242798354f, 0.0256000000f, 0.5830903790f),
			XMFLOAT4(0.5742187500f, 0.1687242798f, 0.2256000000f, 0.7259475219f),
			XMFLOAT4(0.3242187500f, 0.5020576132f, 0.4256000000f, 0.8688046647f),
			XMFLOAT4(0.8242187500f, 0.8353909465f, 0.6256000000f, 0.0320699708f),
			XMFLOAT4(0.1992187500f, 0.2798353909f, 0.8256000000f, 0.1749271137f),
			XMFLOAT4(0.6992187500f, 0.6131687243f, 0.0656000000f, 0.3177842566f),
			XMFLOAT4(0.4492187500f, 0.9465020576f, 0.2656000000f, 0.4606413994f),
			XMFLOAT4(0.9492187500f, 0.0946502058f, 0.4656000000f, 0.6034985423f),
			XMFLOAT4(0.0429687500f, 0.4279835391f, 0.6656000000f, 0.7463556851f),
			XMFLOAT4(0.5429687500f, 0.7613168724f, 0.8656000000f, 0.8892128280f),
			XMFLOAT4(0.2929687500f, 0.2057613169f, 0.1056000000f, 0.0524781341f),
			XMFLOAT4(0.7929687500f, 0.5390946502f, 0.3056000000f, 0.1953352770f),
			XMFLOAT4(0.1679687500f, 0.8724279835f, 0.5056000000f, 0.3381924198f),
			XMFLOAT4(0.6679687500f, 0.3168724280f, 0.7056000000f, 0.4810495627f),
			XMFLOAT4(0.4179687500f, 0.6502057613f, 0.9056000000f, 0.6239067055f),
			XMFLOAT4(0.9179687500f, 0.9835390947f, 0.1456000000f, 0.7667638484f),
			XMFLOAT4(0.1054687500f, 0.0329218107f, 0.3456000000f, 0.9096209913f),
			XMFLOAT4(0.6054687500f, 0.3662551440f, 0.5456000000f, 0.0728862974f),
			XMFLOAT4(0.3554687500f, 0.6995884774f, 0.7456000000f, 0.2157434402f),
			XMFLOAT4(0.8554687500f, 0.1440329218f, 0.9456000000f, 0.3586005831f),
			XMFLOAT4(0.2304687500f, 0.4773662551f, 0.1856000000f, 0.5014577259f),
			XMFLOAT4(0.7304687500f, 0.8106995885f, 0.3856000000f, 0.6443148688f),
			XMFLOAT4(0.4804687500f, 0.2551440329f, 0.5856000000f, 0.7871720117f),
			XMFLOAT4(0.9804687500f, 0.5884773663f, 0.7856000000f, 0.9300291545f),
			XMFLOAT4(0.0273437500f, 0.9218106996f, 0.9856000000f, 0.0932944606f),
			XMFLOAT4(0.5273437500f, 0.0699588477f, 0.0336000000f, 0.2361516035f),
			XMFLOAT4(0.2773437500f, 0.4032921811f, 0.2336000000f, 0.3790087464f),
			XMFLOAT4(0.7773437500f, 0.7366255144f, 0.4336000000f, 0.5218658892f),
			XMFLOAT4(0.1523437500f, 0.1810699588f, 0.6336000000f, 0.6647230321f),
			XMFLOAT4(0.6523437500f, 0.5144032922f, 0.8336000000f, 0.8075801749f),
			XMFLOAT4(0.4023437500f, 0.8477366255f, 0.0736000000f, 0.9504373178f),
			XMFLOAT4(0.9023437500f, 0.2921810700f, 0.2736000000f, 0.1137026239f),
			XMFLOAT4(0.0898437500f, 0.6255144033f, 0.4736000000f, 0.2565597668f),
			XMFLOAT4(0.5898437500f, 0.9588477366f, 0.6736000000f, 0.3994169096f),
			XMFLOAT4(0.3398437500f, 0.1069958848f, 0.8736000000f, 0.5422740525f),
			XMFLOAT4(0.8398437500f, 0.4403292181f, 0.1136000000f, 0.6851311953f),
			XMFLOAT4(0.2148437500f, 0.7736625514f, 0.3136000000f, 0.8279883382f),
			XMFLOAT4(0.7148437500f, 0.2181069959f, 0.5136000000f, 0.9708454810f),
			XMFLOAT4(0.4648437500f, 0.5514403292f, 0.7136000000f, 0.1341107872f),
			XMFLOAT4(0.9648437500f, 0.8847736626f, 0.9136000000f, 0.2769679300f),
			XMFLOAT4(0.0585937500f, 0.3292181070f, 0.1536000000f, 0.4198250729f),
			XMFLOAT4(0.5585937500f, 0.6625514403f, 0.3536000000f, 0.5626822157f),
			XMFLOAT4(0.3085937500f, 0.9958847737f, 0.5536000000f, 0.7055393586f),
			XMFLOAT4(0.8085937500f, 0.0013717421f, 0.7536000000f, 0.8483965015f),
			XMFLOAT4(0.1835937500f, 0.3347050754f, 0.9536000000f, 0.9912536443f),
			XMFLOAT4(0.6835937500f, 0.6680384088f, 0.1936000000f, 0.0145772595f),
			XMFLOAT4(0.4335937500f, 0.1124828532f, 0.3936000000f, 0.1574344023f),
			XMFLOAT4(0.9335937500f, 0.4458161866f, 0.5936000000f, 0.3002915452f),
			XMFLOAT4(0.1210937500f, 0.7791495199f, 0.7936000000f, 0.4431486880f),
			XMFLOAT4(0.6210937500f, 0.2235939643f, 0.9936000000f, 0.5860058309f),
			XMFLOAT4(0.3710937500f, 0.5569272977f, 0.0032000000f, 0.7288629738f),
			XMFLOAT4(0.8710937500f, 0.8902606310f, 0.2032000000f, 0.8717201166f),
			XMFLOAT4(0.2460937500f, 0.0384087791f, 0.4032000000f, 0.0349854227f),
			XMFLOAT4(0.7460937500f, 0.3717421125f, 0.6032000000f, 0.1778425656f),
			XMFLOAT4(0.4960937500f, 0.7050754458f, 0.8032000000f, 0.3206997085f),
			XMFLOAT4(0.9960937500f, 0.1495198903f, 0.0432000000f, 0.4635568513f),
			XMFLOAT4(0.0019531250f, 0.4828532236f, 0.2432000000f, 0.6064139942f),
		};

		return HALTON[index % ArraySize(HALTON)];
	}

	uint32_t CompressNormal(const XMFLOAT3& normal)
	{
		uint32_t retval = 0;

		retval |= (uint32_t)((uint8_t)(normal.x * 127.5f + 127.5f) << 0);
		retval |= (uint32_t)((uint8_t)(normal.y * 127.5f + 127.5f) << 8);
		retval |= (uint32_t)((uint8_t)(normal.z * 127.5f + 127.5f) << 16);

		return retval;
	}
	uint32_t CompressColor(const XMFLOAT3& color)
	{
		uint32_t retval = 0;

		retval |= (uint32_t)((uint8_t)(Saturate(color.x) * 255.0f) << 0);
		retval |= (uint32_t)((uint8_t)(Saturate(color.y) * 255.0f) << 8);
		retval |= (uint32_t)((uint8_t)(Saturate(color.z) * 255.0f) << 16);

		return retval;
	}
	uint32_t CompressColor(const XMFLOAT4& color)
	{
		uint32_t retval = 0;

		retval |= (uint32_t)((uint8_t)(Saturate(color.x) * 255.0f) << 0);
		retval |= (uint32_t)((uint8_t)(Saturate(color.y) * 255.0f) << 8);
		retval |= (uint32_t)((uint8_t)(Saturate(color.z) * 255.0f) << 16);
		retval |= (uint32_t)((uint8_t)(Saturate(color.w) * 255.0f) << 24);

		return retval;
	}
}