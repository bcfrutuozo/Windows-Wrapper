#pragma once

#include "Common.h"
#include "IEquatable.h"
#include "Mathlib.h"

struct Color : public IEquatable<Color>
{
	uint32_t rgba = 0;

	constexpr Color(uint32_t rgba) : rgba(rgba) {}
	constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) : rgba((r << 0) | (g << 8) | (b << 16) | (a << 24)) {}

	inline bool Equals(Color c) const noexcept override
	{
		return rgba == c.rgba;
	}

	uint8_t GetR() const { return (rgba >> 0) & 0xFF; }
	uint8_t GetG() const { return (rgba >> 8) & 0xFF; }
	uint8_t GetB() const { return (rgba >> 16) & 0xFF; }
	uint8_t GetA() const { return (rgba >> 24) & 0xFF; }

	void SetR(uint8_t value) { *this = Color(value, GetG(), GetB(), GetA()); }
	void SetG(uint8_t value) { *this = Color(GetR(), value, GetB(), GetA()); }
	void SetB(uint8_t value) { *this = Color(GetR(), GetG(), value, GetA()); }
	void SetA(uint8_t value) { *this = Color(GetR(), GetG(), GetB(), value); }

	constexpr XMFLOAT3 ToFloat3() const
	{
		return XMFLOAT3(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f);
	}

	constexpr XMFLOAT4 ToFloat4() const
	{
		return XMFLOAT4(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f, ((rgba >> 24) & 0xFF) / 255.0f);
	}

	constexpr operator XMFLOAT3() const { return ToFloat3(); }
	constexpr operator XMFLOAT4() const { return ToFloat4(); }

	static constexpr Color FromFloat3(const XMFLOAT3& value)
	{
		return Color(static_cast<uint8_t>(value.x * 255), static_cast<uint8_t>(value.y * 255), static_cast<uint8_t>(value.z * 255));
	}

	static constexpr Color FromFloat4(const XMFLOAT4& value)
	{
		return Color(static_cast<uint8_t>(value.x * 255), static_cast<uint8_t>(value.y * 255), static_cast<uint8_t>(value.z * 255), static_cast<uint8_t>(value.w * 255));
	}

	static constexpr Color Lerp(Color a, Color b, float i)
	{
		return FromFloat4(Math::Lerp(a.ToFloat4(), b.ToFloat4(), i));
	}

	static constexpr Color Red() { return Color(255, 0, 0, 255); }
	static constexpr Color Green() { return Color(0, 255, 0, 255); }
	static constexpr Color Blue() { return Color(0, 0, 255, 255); }
	static constexpr Color Black() { return Color(0, 0, 0, 255); }
	static constexpr Color White() { return Color(255, 255, 255, 255); }
	static constexpr Color Yellow() { return Color(255, 255, 0, 255); }
	static constexpr Color Purple() { return Color(255, 0, 255, 255); }
	static constexpr Color Cyan() { return Color(0, 255, 255, 255); }
	static constexpr Color Transparent() { return Color(0, 0, 0, 0); }
	static constexpr Color Gray() { return Color(127, 127, 127, 255); }
	static constexpr Color Ghost() { return Color(127, 127, 127, 127); }
};