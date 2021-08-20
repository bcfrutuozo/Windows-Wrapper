#pragma once

#include "MenuItem.h"

class MenuCheckItem : public MenuItem
{
protected:

	void Initialize() override;

public:

	bool IsChecked;

	MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuCheckItem() = default;

	void Update() override;
};