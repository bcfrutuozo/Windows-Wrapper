#include "Menu.h"

// MenuItem
MenuItem::MenuItem(Control* parent, Type type, const std::string& text, IEvent* callback)
	:
	Control(parent, text),
	m_Type(type),
	m_Callback(callback)
{	}

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

void Menu::SetText(const std::string& text)
{
	Text = text;
}

void Menu::AddEntry(MenuItem::Type type, const std::string& text, IEvent* callback)
{
	AppendMenu(static_cast<HMENU>(Handle.ToPointer()), MF_STRING, callback->GetId(), text.c_str());
	m_SubEntries.push_back(MenuItem(this, type, text, callback));
	SetMenu(static_cast<HWND>(Parent->Parent->Handle.ToPointer()), static_cast<HMENU>(Parent->Handle.ToPointer()));
	// Pointer to parent->parent (window), parent (menubar)
}

// MenuBar
MenuBar::MenuBar(Control* parent)
	:
	MenuBar(parent, "")
{

};

MenuBar::MenuBar(Control* parent, const std::string& text)
	:
	Control(parent, text)
{
	Handle = CreateMenu();
	Menu m = Menu(this, text);
	m_Entries.push_back(m);
	AppendMenu(static_cast<HMENU>(Handle.ToPointer()), MF_POPUP, (UINT_PTR)m.GetHandle().ToPointer(), Text.c_str());
	SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), static_cast<HMENU>(Handle.ToPointer()));
};

MenuBar::~MenuBar()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

void MenuBar::SetText(const std::string& text)
{
	Text = text;
}