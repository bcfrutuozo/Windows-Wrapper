#pragma once

#include "MenuItem.h"

class MenuRadioItem : public MenuItem
{
private:

	// Invalidates the whole Radio buttons section within a submenu
	std::tuple<int, int> InvalidateSection(int section);

protected:

	// Dispatch is overridden to process the Radio submenu check after the callback
	void Dispatch(const std::string& name) override;

public:

	bool IsChecked;

	MenuRadioItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuRadioItem() = default;

	void Bind() noexcept override;
};