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

	// Delete AddMenu/AddCheckItem/AddRadioItem/AddSeparator because menu bar can only have Menu and MenuItem
	Menu& AddMenu(const std::string& text) = delete;
	MenuItem& AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddCheckItem(const std::string& text, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, const std::function<void()>& function, bool isChecked) = delete;
	MenuItem& AddRadioItem(const std::string& text, bool isChecked) = delete;
	void AddSeparator() = delete;

	virtual void Bind() noexcept override;

	void OnClickSet(const std::function<void()>& function);
	void OnClick() const;
};