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

	// Fields for menu/submenu indexing
	unsigned int m_Id;
	unsigned int m_SubItemIndex;
	int m_Section;

	// std::vector<std::shared_ptr<Menu>> m_MenuItems;

	// Fields for callback binding
	std::unique_ptr<Event<>> m_ClickDelegate;
	static unsigned int m_CurrentIndex;
	
	std::tuple<int, int> InvalidateSection(int section);
	virtual void DispatchEvent(unsigned int id);

public:

	Menu(Control* parent, unsigned int subitemIndex, int section);
	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section);
	virtual ~Menu();

	virtual void Bind() noexcept override;

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

	// For MenuSeparator
	void AddSeparator();
};