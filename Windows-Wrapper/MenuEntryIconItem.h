#pragma once

#include "MenuItem.h"

class MenuEntryIconItem : public MenuItem
{
protected:

	// Will be loaded in the constructor if the file path is not empty
	HBITMAP m_Icon = nullptr;

	void Initialize() noexcept override;

public:

	MenuEntryIconItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath);
	virtual ~MenuEntryIconItem();
};

