#include "Menu.h"

Menu::Menu(Control* parent)
	:
	Control(parent, ""),
	m_MenuBar(CreateMenu()),
	m_Menu(CreateMenu())
{
	AppendMenu(m_MenuBar, MF_POPUP, (UINT_PTR)m_Menu, Text.c_str());
	SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), m_MenuBar);
};

Menu::Menu(Control* parent, const std::string& text)
	:
	Control(parent, text),
	m_MenuBar(CreateMenu()),
	m_Menu(CreateMenu())
{
	AppendMenu(m_MenuBar, MF_POPUP, (UINT_PTR)m_Menu, Text.c_str());
	SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), m_MenuBar);
};

Menu::~Menu()
{
	DestroyMenu(m_Menu);
	DestroyMenu(m_MenuBar);
}

void Menu::SetText(const std::string& text)
{
	Text = text;
}

void Menu::AddEntry(MenuItem::Type type, const std::string& text, IEvent* callback)
{
	MenuItem(type, text, callback);
}