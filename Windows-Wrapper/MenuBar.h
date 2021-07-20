#pragma once

#include "Menu.h"

class Window;

class MenuBar : public Menu
{
	friend class Window;

public:

	MenuBar(Control* parent);
	~MenuBar();

	MenuItem& AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath = { }) override;
	MenuItem& AddItem(const std::string& text, const std::string& iconPath = { }) override;
	void SetText(const std::string& text) override;
};