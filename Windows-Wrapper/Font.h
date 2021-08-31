#pragma once

#include "Object.h"
#include "Enums.h"

class Font : public Object
{
private:

	bool m_IsBold;
	bool m_IsItalic;
	bool m_IsUnderline;
	bool m_IsStrikeOut;
	std::string m_Name;
	int m_Size;
	GraphicsUnit m_Unit;

	static int PixelToPoint(int sizeInPixels) noexcept;
	static int PointToPixel(int sizeInPoints) noexcept;

public:

	static constexpr unsigned int DefaultHeight() { return 9; }

	Font(const std::string& name, const int& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout, GraphicsUnit unit);
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
};

