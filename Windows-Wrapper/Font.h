#pragma once

#include "Base.h"
#include "Enums.h"

class Font : public Base
{
private:

	bool m_IsBold;
	bool m_IsItalic;
	bool m_IsUnderline;
	bool m_IsStrikeOut;
	std::string m_Name;
	float m_Size;
	GraphicsUnit m_Unit;

public:

	Font(const std::string& name, const float& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout, GraphicsUnit unit);
	virtual ~Font() = default;

	bool IsBold() const noexcept;
	bool IsItalic() const noexcept;
	bool IsUnderline() const noexcept;
	bool IsStrikeOut() const noexcept;
	float GetSize() const noexcept;
	void SetSize(const float& size) noexcept;
	std::string GetName() const noexcept;
	GraphicsUnit GetUnit() const noexcept;
	void SetStyle(FontStyle style) noexcept;
};

