#pragma once

#include "MenuItem.h"

class MenuCheckItem : public MenuItem
{
private:

	virtual void OnInternalUpdate(Control* const sender, EventArgs* const args) override;

protected:

	void Initialize() noexcept override;

public:

	bool IsChecked;

	MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuCheckItem() = default;
};