#include "Font.h"

Font::Font(const std::string& name, const float& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout)
	:
	IsBold(isBold),
	IsItalic(isItalic),
	Name(name),
	Size(size),
	IsUnderline(isUnderline),
	IsStrikeout(isStrikeout)
{
}