#include "Menu.h"

#include <filesystem>

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

// MenuItem
MenuItem::MenuItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, iconPath)
{
	m_Id = i;
	m_ClickDelegate = std::make_unique<Event<>>(function);
}

void MenuItem::Bind()
{
	if (!m_IconPath.empty())
	{
		m_Icon = (HBITMAP)LoadImage(NULL, m_IconPath.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
		InsertMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_POPUP, m_Id, Text.c_str());
		MENUITEMINFO mi = { 0 };
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_BITMAP;
		mi.hbmpItem = m_Icon;
		bool x =SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
		//mi.dwTypeData = const_cast<char*>(Text.c_str());
		//bool x =InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), (UINT)Handle.ToPointer(), FALSE, &mi);
		auto hr = GetLastError();
		bool a = x;
	}
	else
	{
		MENUITEMINFO mi = { 0 };
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_STRING | MIIM_ID;
		mi.wID = m_Id;
		mi.dwTypeData = const_cast<char*>(Text.c_str());
		InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), (UINT)Handle.ToPointer(), FALSE, &mi);
		//AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_STRING, m_Id, Text.c_str());
	}
}

Menu& Menu::AddMenu(const std::string& text)
{
	auto m = std::make_shared<Menu>(this, text, static_cast<unsigned int>(m_MenuItems.size()), ""); // Menubar doesn't have icons
	m->Bind();
	m_MenuItems.push_back(m);
	return *m_MenuItems.back();
}

// Menu
Menu::Menu(Control* parent, unsigned int subitemIndex, const std::string& iconPath)
	:
	Menu(parent, "", subitemIndex, iconPath)
{
	
};

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, const std::string& iconPath)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_IconPath(iconPath)
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

void Menu::Bind()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mi.wID = m_Id;
	mi.hSubMenu = (HMENU)Handle.ToPointer();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), (UINT)Handle.ToPointer(), FALSE, &mi);
	//AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_POPUP, (UINT_PTR)Handle.ToPointer(), Text.c_str());
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
		if (typeid(ch) != typeid(MenuItem))
		{
			ch->DispatchEvent(id);
		}
	}
};

MenuSeparator::MenuSeparator(Menu* parent, unsigned int subitemIndex)
	:
	Menu(parent, subitemIndex)
{
	m_Id = 0;
}

void MenuSeparator::Bind()
{
	AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_SEPARATOR, 0, 0);
}