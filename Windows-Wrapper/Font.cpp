#include "Font.h"

int Font::PixelToPoint(int sizeInPixels) noexcept
{
	HDC hdc = CreateCompatibleDC(nullptr);
	int value = std::abs(MulDiv(sizeInPixels, 72, GetDeviceCaps(hdc, LOGPIXELSY)));
	DeleteDC(hdc);
	return value;
}

int Font::PointToPixel(int sizeInPoints) noexcept
{
	HDC hdc = CreateCompatibleDC(nullptr);
	int pointPerInch = 72;
	int deviceCaps = GetDeviceCaps(hdc, LOGPIXELSY);
	int fntSize;
	fntSize = -MulDiv(sizeInPoints, deviceCaps, pointPerInch);
	DeleteDC(hdc);
	return std::abs(fntSize);
}

Font::Font(const std::string& name, const int& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout, GraphicsUnit unit)
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

int Font::GetSize() const noexcept
{
	return m_Size;
}

int Font::GetSizeInPixels() const noexcept
{
	switch (m_Unit)
	{
	case GraphicsUnit::World:
	{
		break;
	}
	case GraphicsUnit::Display:
	{
		break;
	}
	case GraphicsUnit::Pixel:
	{
		return m_Size;
	}
	case GraphicsUnit::Point:
	{
		return PointToPixel(m_Size);
	}
	case GraphicsUnit::Inch:
	{
		break;
	}
	case GraphicsUnit::Document:
	{
		break;
	}
	case GraphicsUnit::Millimeter:
	{
		break;
	}
	}
}

void Font::SetSize(const int& size) noexcept
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

void Font::SetUnit(GraphicsUnit unit) noexcept
{
	switch (unit)
	{
	case GraphicsUnit::World:
	{
		break;
	}
	case GraphicsUnit::Display:
	{
		break;
	}
	case GraphicsUnit::Pixel:
	{
		switch (m_Unit)
		{
		case GraphicsUnit::Point:	// POINT TO PIXEL
		{
			m_Size = PointToPixel(m_Size);
			break;
		}
		}
		break;
	}
	case GraphicsUnit::Point:	
	{
		switch (m_Unit)
		{
		case GraphicsUnit::Pixel:	// PIXEL TO POINT
		{
			m_Size = PixelToPoint(m_Size);
			break;
		}
		}
		break;
	}
	case GraphicsUnit::Inch:
	{
		break;
	}
	case GraphicsUnit::Document:
	{
		break;
	}
	case GraphicsUnit::Millimeter:
	{
		break;
	}
	}

	m_Unit = unit;
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
