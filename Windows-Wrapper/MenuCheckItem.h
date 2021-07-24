#pragma once

#include "MenuItem.h"

class MenuCheckItem : public MenuItem
{
protected:

	// Dispatch is overridden to process the Check on the item after the callback
	void Dispatch(const std::string& name) override;

public:

	bool IsChecked;

	MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuCheckItem() = default;

	void Bind() noexcept override;
};