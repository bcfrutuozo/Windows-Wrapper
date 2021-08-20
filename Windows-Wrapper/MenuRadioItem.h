#pragma once

#include "MenuItem.h"

class MenuRadioItem : public MenuItem
{
private:

	// Invalidates the whole Radio buttons section within a submenu
	std::tuple<int, int> InvalidateSection(int section);

protected:

	void Initialize() override;

public:

	bool IsChecked;

	MenuRadioItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuRadioItem() = default;

	void Update() override;
};