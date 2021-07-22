#pragma once

#include "Menu.h"

class Window;

class MenuBar : public Menu
{
	friend class Window;

public:

	MenuBar(Control* parent);
	~MenuBar();

	// Delete AddCheckItem because menu bar can only have Menu and MenuItem
	MenuItem& AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddCheckItem(const std::string& text, bool isChecked) = delete;
};