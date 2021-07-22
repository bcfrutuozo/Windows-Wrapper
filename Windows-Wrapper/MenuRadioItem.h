#pragma once

#include "MenuItem.h"

class MenuRadioItem : public MenuItem
{
public:

	bool IsSelected;

	MenuRadioItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, bool isSelected);
	virtual ~MenuRadioItem();

	void Bind() override;
	void DispatchEvent(unsigned int id);
	void SetText(const std::string& text) override;
};

