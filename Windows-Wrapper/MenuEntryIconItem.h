#pragma once

#include "MenuItem.h"

class MenuEntryIconItem : public MenuItem
{
private:

	// Will be loaded in the constructor if the file path is not empty
	HBITMAP m_Icon = nullptr;

protected:

	void Initialize() override;

public:

	MenuEntryIconItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath);
	virtual ~MenuEntryIconItem();
};

