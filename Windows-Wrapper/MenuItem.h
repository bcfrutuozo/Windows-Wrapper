#pragma once

#include "Menu.h"

class MenuItem : public Menu
{
public:

	MenuItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();

	virtual void Bind() override;
	virtual void SetText(const std::string& text) override;

	void OnClickSet(const std::function<void()>& function);
	void OnClick() const;
};