#pragma once

#include "Menu.h"
#include "IActivable.h"

class MenuItem : public Menu, public IActivable
{
public:

	MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();

	void Disable() noexcept override;
	void Enable() noexcept override;
};