#pragma once

#include "Base.h"
#include "Enums.h"

class Font : public Base
{
private:

public:

	bool IsBold;
	bool IsItalic;
	bool IsUnderline;
	bool IsStrikeout;
	std::string Name;
	float Size;

	Font(const std::string& name, const float& size, bool isBold, bool isItalic, bool isUnderline, bool isStrikeout);
	virtual ~Font() = default;
};

