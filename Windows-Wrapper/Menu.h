#pragma once

#include "Control.h"
#include "Event.h"

#include <memory>

class MenuItem;

class Menu : public Control
{
	friend class Window;
	friend class MenuCheckItem;
	friend class MenuRadioItem;

protected:

	unsigned int m_Id;
	unsigned int m_SubItemIndex;
	int m_Section;
	std::string m_IconPath;
	HBITMAP m_Icon;
	std::vector<std::shared_ptr<Menu>> m_MenuItems;
	std::unique_ptr<Event<>> m_ClickDelegate;
	static unsigned int m_CurrentIndex;
	
	std::tuple<int, int> InvalidateSection(int section);

	virtual unsigned int GetId();	
	virtual void DispatchEvent(unsigned int id);

public:

	Menu(Control* parent, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath = { });
	virtual ~Menu();

	virtual void Bind();
	void AddSeparator();

	// For MenuBar
	Menu& AddMenu(const std::string& text);

	// For MenuItem
	MenuItem& AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath = {});
	MenuItem& AddItem(const std::string& text, const std::string& iconPath = { });

	// For MenuCheckItem
	MenuItem& AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked);
	MenuItem& AddCheckItem(const std::string& text, bool isChecked);

	// For MenuRadioItem
	MenuItem& AddRadioItem(const std::string& text, const std::function<void()>& function, bool isChecked);
	MenuItem& AddRadioItem(const std::string& text, bool isChecked);

	void SetText(const std::string& text) override;
};