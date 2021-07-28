#pragma once

#include "MenuRoot.h"

class MenuLeaf : public MenuRoot
{
public:

	MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section);
	virtual ~MenuLeaf() = default;

	MenuItem& AddItemWithIcon(const std::string& text, const std::string& iconPath);
	MenuItem& AddCheckItem(const std::string& text, bool isChecked);
	MenuItem& AddRadioItem(const std::string& text, bool isChecked);
	void AddSeparator();
};

