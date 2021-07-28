#pragma once

#include "Menu.h"

class MenuItem : public Menu
{
public:

	MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();
};