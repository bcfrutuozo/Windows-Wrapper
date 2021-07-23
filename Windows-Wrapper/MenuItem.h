#pragma once

#include "Menu.h"

class MenuItem : public Menu
{
protected:

	std::string m_IconPath;
	HBITMAP m_Icon;

public:

	MenuItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~MenuItem();

	virtual void Bind() noexcept override;

	void OnClickSet(const std::function<void()>& function);
	void OnClick() const;
};