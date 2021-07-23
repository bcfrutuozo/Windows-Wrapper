#pragma once

#include "MenuItem.h"

class MenuRadioItem : public MenuItem
{
public:

	bool IsSelected;

	MenuRadioItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, bool isSelected);
	virtual ~MenuRadioItem() = default;

	void Bind() noexcept override;
	void DispatchEvent(unsigned int id);
};