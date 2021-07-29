#pragma once

#include "MenuRoot.h"
#include "IActivableMenu.h"

class MenuLeaf : public MenuRoot, public IActivableMenu
{
public:

	MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section);
	virtual ~MenuLeaf() = default;


	void Enable() noexcept;
	void Disable() noexcept;
	void Gray() noexcept;

	MenuItem& AddItemWithIcon(const std::string& text, const std::string& iconPath);
	MenuItem& AddCheckItem(const std::string& text, bool isChecked);
	MenuItem& AddRadioItem(const std::string& text, bool isChecked);
	void AddSeparator();
};

