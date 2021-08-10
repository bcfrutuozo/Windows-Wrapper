#include "Font.h"

Font::Font(const std::string& name, const float& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout, GraphicsUnit unit)
	:
	m_IsBold(isBold),
	m_IsItalic(isItalic),
	m_Name(name),
	m_Size(size),
	m_IsUnderline(isUnderline),
	m_IsStrikeOut(isStrikeout),
	m_Unit(unit)
{
}

bool Font::IsBold() const noexcept
{
	return m_IsBold;
}

bool Font::IsItalic() const noexcept
{
	return m_IsItalic;
}

bool Font::IsUnderline() const noexcept
{
	return m_IsUnderline;
}

bool Font::IsStrikeOut() const noexcept
{
	return m_IsStrikeOut;
}

float Font::GetSize() const noexcept
{
	switch (m_Unit)
	{
	case GraphicsUnit::World: break;
	case GraphicsUnit::Display: break;
	case GraphicsUnit::Pixel:
	{
		return m_Size;
		break;
	}
	case GraphicsUnit::Point:
	{
		HDC hdc = CreateCompatibleDC(0);
		int pointPerInch = 72;
		int deviceCaps = GetDeviceCaps(hdc, LOGPIXELSY);
		// Generate a LOGFONT containing all the font properties.
		LOGFONT logFont;
		strcpy_s(logFont.lfFaceName, m_Name.c_str());
		logFont.lfHeight = -MulDiv(m_Size, deviceCaps, pointPerInch);
		logFont.lfWidth = 0; // keep zero.
		DeleteDC(hdc);
		return logFont.lfHeight;
	}
	case GraphicsUnit::Inch: break;
	case GraphicsUnit::Document: break;
	case GraphicsUnit::Millimeter:break;
	}
}

void Font::SetSize(const float& size) noexcept
{
	m_Size = size;
}

std::string Font::GetName() const noexcept
{
	return m_Name;
}

GraphicsUnit Font::GetUnit() const noexcept
{
	return m_Unit;
}

void Font::SetStyle(FontStyle style) noexcept
{
	switch (style)
	{
	case FontStyle::Regular:
	{
		m_IsBold = m_IsItalic = m_IsUnderline = m_IsStrikeOut = false;
		break;
	}
	case FontStyle::Bold:
	{
		m_IsBold = true;
		break;
	}
	case FontStyle::Italic:
	{
		m_IsItalic = true;
		break;
	}
	case FontStyle::Underline:
	{
		m_IsUnderline = true;
		break;
	}
	case FontStyle::Strikeout:
	{
		m_IsStrikeOut = true;
		break;
	}
	}
}
