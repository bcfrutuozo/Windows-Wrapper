#pragma once

#include "IEquatable.h"

struct Color : public IEquatable<Color>
{
private:

	uint32_t rgba = 0;

public:

	constexpr Color(uint32_t rgba) noexcept : rgba(rgba) {}
	constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) noexcept : rgba((r << 0) | (g << 8) | (b << 16) | (a << 24)) {}

	inline constexpr bool operator==(const Color& c) const { return rgba == c.rgba; }

	inline constexpr uint8_t GetR() const { return (rgba >> 0) & 0xFF; }
	inline constexpr uint8_t GetG() const { return (rgba >> 8) & 0xFF; }
	inline constexpr uint8_t GetB() const { return (rgba >> 16) & 0xFF; }
	inline constexpr uint8_t GetA() const { return (rgba >> 24) & 0xFF; }

	void SetR(uint8_t value) noexcept;
	void SetG(uint8_t value) noexcept;
	void SetB(uint8_t value) noexcept;
	void SetA(uint8_t value) noexcept;

	constexpr XMFLOAT3 ToFloat3() const
	{
		return XMFLOAT3(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f);
	}

	constexpr XMFLOAT4 ToFloat4() const
	{
		return XMFLOAT4(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f, ((rgba >> 24) & 0xFF) / 255.0f);
	}

	constexpr D3DCOLORVALUE ToD3DColor() const
	{
		return D3DCOLORVALUE(((rgba >> 0) & 0xFF) / 255.0f, ((rgba >> 8) & 0xFF) / 255.0f, ((rgba >> 16) & 0xFF) / 255.0f, ((rgba >> 24) & 0xFF) / 255.0f);
	}

	constexpr uint32_t ToRGB() const
	{
		return rgba & 0x00FFFFFF;
	}

	constexpr uint32_t ToRGBA() const
	{
		return rgba;
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

	// Windows defaul control color
	static constexpr Color Border() { return Color(173, 173, 173, 255); }
	static constexpr Color ControlBackground_Win10() { return Color(240, 240, 240, 255); }
	static constexpr Color ControlBackground_Win11() { return Color(255, 255, 255, 255); }
	static constexpr Color Default() { return Color(212, 208, 200, 255); }
	static constexpr Color SelectionBackground() { return Color(0, 120, 215, 255); }
	static constexpr Color SelectionForeground() { return Color(255, 255, 255, 255); }
	static constexpr Color WindowBackground() { return Color(247, 247, 247, 255); }
	static constexpr Color Foreground() { return Color(0, 0, 0, 255); }
	static constexpr Color DisabledControlBackground() { return Color(247, 247, 247, 255); }
	static constexpr Color DisabledForeground() { return Color(82, 86, 82, 255); }

	int GetHashCode() const override;
	inline bool Equals(const Object* const c) const override;
	inline bool Equals(const Color* const c) const override;
	const std::string ToString() const noexcept override;
};