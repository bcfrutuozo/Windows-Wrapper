#pragma once

#include "MenuItem.h"

class MenuEntryItem : public MenuItem
{
protected:

	void Initialize() noexcept override;

public:

	MenuEntryItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section);
	virtual ~MenuEntryItem() = default;
};

