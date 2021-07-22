#pragma once

#include "Menu.h"

class MenuSeparator : public Menu
{
public:

	MenuSeparator(Menu* parent, unsigned int subitemIndex);

	void Bind() noexcept override;

	// Delete AddMenu/AddCheckItem/AddRadioItem/AddSeparator because menu bar can only have Menu and MenuItem
	Menu& AddMenu(const std::string& text) = delete;
	MenuItem& AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddCheckItem(const std::string& text, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, bool isChecked) = delete;
	void AddSeparator() = delete;
};