#pragma once

#include "Menu.h"
#include "IActivableMenu.h"

class MenuItem : public Menu, public IActivableMenu
{
public:

	MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();

	void Enable() noexcept override;
	void Disable() noexcept override;
	void Gray() noexcept override;
};