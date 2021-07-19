#pragma once

#include "Menu.h"

class MenuBar : public Menu
{

public:

	MenuBar(Control* parent);

	//Menu& AddMenu(const std::string& text);

	virtual MenuItem& AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath = { }) override;
	virtual MenuItem& AddItem(const std::string& text, const std::string& iconPath = { }) override;
	void SetText(const std::string& text) override;
};