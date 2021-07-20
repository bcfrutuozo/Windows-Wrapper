#pragma once

#include "Control.h"
#include "Event.h"

#include <memory>

class MenuItem;

class Menu : public Control
{
	friend class Window;

protected:

	unsigned int m_Id;
	unsigned int m_SubItemIndex;
	std::string m_IconPath;
	HBITMAP m_Icon;
	std::vector<std::shared_ptr<Menu>> m_MenuItems;
	std::unique_ptr<Event<>> m_ClickDelegate;
	static unsigned int m_CurrentIndex;
	
	virtual unsigned int GetId();	
	virtual void DispatchEvent(unsigned int id) const;

public:

	Menu(Control* parent, unsigned int subitemIndex, const std::string& iconPath = { });
	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, const std::string& iconPath = { });
	~Menu();
	virtual void Bind();

	void AddSeparator();
	virtual Menu& AddMenu(const std::string& text);
	virtual MenuItem& AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath = {});
	virtual MenuItem& AddItem(const std::string& text, const std::string& iconPath = { });
	void SetText(const std::string& text) override;
};