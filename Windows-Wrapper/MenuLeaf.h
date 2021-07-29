#pragma once

#include "MenuRoot.h"
#include "IActivableMenu.h"

class MenuLeaf : public MenuRoot, public IActivableMenu
{
public:

	MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section);
	virtual ~MenuLeaf() = default;

	void Disable() noexcept override;
	void Enable() noexcept override;

	MenuItem& AddItemWithIcon(const std::string& text, const std::string& iconPath);
	MenuItem& AddCheckItem(const std::string& text, bool isChecked);
	MenuItem& AddRadioItem(const std::string& text, bool isChecked);
	void AddSeparator();
};

