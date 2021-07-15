#include "Menu.h"

// MenuItem
MenuItem::MenuItem(Control* parent, const std::string& text, IEvent* callback)
	:
	Menu(parent, text)
{
	//Events.Register(callback);
}

void MenuItem::Bind()
{
	AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_STRING, 0, Text.c_str());
}

// Menu
Menu::Menu(Control* parent)
	:
	Menu(parent, "")
{

};

Menu::Menu(Control* parent, const std::string& text)
	:
	Control(parent, text)
{
	Handle = CreateMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

void Menu::Bind()
{
	AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_POPUP, (UINT_PTR)Handle.ToPointer(), Text.c_str());
}

void Menu::SetText(const std::string& text)
{
	Text = text;
}

MenuItem& Menu::AddItem(const std::string& text, IEvent* callback)
{
	MenuItem* m = new MenuItem(this, text, callback);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}

// MenuBar
MenuBar::MenuBar()
	:
	Control(nullptr)
{
	Handle = CreateMenu();
}

MenuBar::MenuBar(Control* parent)
	:
	Control(parent, "")
{
	Handle = CreateMenu();
};

MenuBar::~MenuBar()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

Menu& MenuBar::AddMenu(const std::string& text)
{
	Menu* m = new Menu(this, text);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}

MenuItem& MenuBar::AddItem(const std::string& text, IEvent* callback)
{
	MenuItem* m = new MenuItem(this, text, callback);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}

void MenuBar::SetText(const std::string& text)
{
	Text = text;
}