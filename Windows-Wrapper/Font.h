#pragma once

#include "Object.h"

#include <string>

enum class GenericFontFamilies
{
	Serif = 0,
	SansSerif = 1,
	Monospace = 2
};

enum class FontStyle
{
	Regular = 0,
	Bold = 1,
	Italic = 2,
	Underline = 4,
	Strikeout = 8
};

enum class GraphicsUnit
{
	World = 0,
	Display = 1,
	Pixel = 2,
	Point = 3,
	Inch = 4,
	Document = 5,
	Millimeter = 6
};

class Font final: public Object
{
private:

	bool m_IsBold;
	bool m_IsItalic;
	bool m_IsUnderline;
	bool m_IsStrikeOut;
	std::string m_Name;
	int m_Size;
	GraphicsUnit m_Unit;
	unsigned char m_GdiCharSet;
	bool m_GdiVerticalFont;

	static int PixelToPoint(int sizeInPixels) noexcept;
	static int PointToPixel(int sizeInPoints) noexcept;

public:

	static constexpr unsigned int DefaultHeight() { return 9; }

	Font(const std::string& name, int size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout, GraphicsUnit unit);
	virtual ~Font() = default;

	bool IsBold() const noexcept;
	bool IsItalic() const noexcept;
	bool IsUnderline() const noexcept;
	bool IsStrikeOut() const noexcept;
	int GetSize() const noexcept;
	int GetSizeInPixels() const noexcept;
	void SetSize(const int& size) noexcept;
	std::string GetName() const noexcept;
	GraphicsUnit GetUnit() const noexcept;
	void SetUnit(GraphicsUnit unit) noexcept;
	void SetStyle(FontStyle style) noexcept;
	std::string ToString() const noexcept override;
};