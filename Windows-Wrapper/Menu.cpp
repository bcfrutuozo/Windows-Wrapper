#include "Menu.h"

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

// MenuItem
MenuItem::MenuItem(Control* parent, const std::string& text, const std::function<void()>& function, unsigned int i)
	:
	Menu(parent, text)
{
	m_Id = i;
	m_ClickDelegate = new Event<>(function);
}

void MenuItem::Bind()
{
	AppendMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), MF_STRING, m_Id, Text.c_str());
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

MenuItem& Menu::AddItem(const std::string& text, const std::function<void()>& function)
{
	MenuItem* m = new MenuItem(this, text, function, m_CurrentIndex++);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}

MenuItem& Menu::AddItem(const std::string& text)
{
	return AddItem(text, nullptr);
}

void Menu::DispatchEvent(unsigned int id) const
{
	for (auto& ch : m_MenuItems)
	{
		if (ch.m_Id == id)
		{
			return ch.m_ClickDelegate->Trigger();
		}

		if (typeid(ch) != typeid(MenuItem) || ch.m_MenuItems.size() > 0)
		{
			return ch.DispatchEvent(id);
		}
	}
};