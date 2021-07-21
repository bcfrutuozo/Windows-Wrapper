#include "Menu.h"
#include "MenuSeparator.h"
#include "MenuItem.h"

#include <filesystem>

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

Menu::Menu(Control* parent, unsigned int subitemIndex, const std::string& iconPath)
	:
	Menu(parent, "", subitemIndex, iconPath)

{
	
};

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, const std::string& iconPath)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_IconPath(iconPath),
	m_Id(0)				// No processing for default menu.
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

Menu& Menu::AddMenu(const std::string& text)
{
	auto m = std::make_shared<Menu>(this, text, static_cast<unsigned int>(m_MenuItems.size()), ""); // Menubar doesn't have icons
	m->Bind();
	m_MenuItems.push_back(m);
	return *m_MenuItems.back();
}

void Menu::Bind()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mi.wID = m_Id;
	mi.hSubMenu = (HMENU)Handle.ToPointer();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), Handle.ToInt32(), FALSE, &mi);
}

void Menu::SetText(const std::string& text)
{
	Text = text;
}

MenuItem& Menu::AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath)
{
	auto m = std::make_shared<MenuItem>(this, text, function, m_CurrentIndex++, m_MenuItems.size(), iconPath);
	m->Bind();
	m_MenuItems.push_back(m);
	return dynamic_cast<MenuItem&>(*m_MenuItems.back());
}

MenuItem& Menu::AddItem(const std::string& text, const std::string& iconPath)
{
	return AddItem(text, nullptr, iconPath);
}

void Menu::AddSeparator()
{
	auto m = std::make_shared<MenuSeparator>(this, m_MenuItems.size());
	m->Bind();
	m_MenuItems.push_back(m);
}

void Menu::DispatchEvent(unsigned int id) const
{
	if (m_Id == id)
	{
		return m_ClickDelegate->Trigger();
	}

	for (auto& ch : m_MenuItems)
	{
		if (ch->GetType() == typeid(MenuItem) || ch->GetType() == typeid(Menu))
		{
			ch->DispatchEvent(id);
		}
	}
};

unsigned int Menu::GetId()
{
	return 0;
}