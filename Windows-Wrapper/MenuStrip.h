#pragma once

#include "Menu.h"

class Window;

class MenuStrip : public Menu
{
	friend class Window;

public:

	MenuStrip(Control* parent);

	void Bind() noexcept override;

	// Delete AddCheckItem/AddRadioItem/AddSeparator because menu bar can only have Menu and MenuItem
	MenuItem& AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddCheckItem(const std::string& text, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, bool isChecked) = delete;
	void AddSeparator() = delete;
};