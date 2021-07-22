#pragma once

#include "MenuItem.h"

class MenuCheckItem : public MenuItem
{
public:

	bool IsChecked;

	MenuCheckItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, bool isChecked);
	virtual ~MenuCheckItem();

	void Bind() override;
	void DispatchEvent(unsigned int id);
	void SetText(const std::string& text) override;
};

