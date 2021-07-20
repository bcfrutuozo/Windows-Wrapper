#pragma once

#include "Menu.h"

class MenuSeparator : public Menu
{
public:

	MenuSeparator(Menu* parent, unsigned int subitemIndex);
	~MenuSeparator();

	void Bind() override;
};