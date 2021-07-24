#pragma once

#include "Menu.h"

class MenuItem : public Menu
{
protected:

	// Will be loaded in the constructor if the file path is not empty
	HBITMAP m_Icon = nullptr;

public:

	MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();

	virtual void Bind() noexcept override;
};