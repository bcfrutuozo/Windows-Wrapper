#include "Menu.h"

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

// MenuItem
MenuItem::MenuItem(Control* parent, const std::string& text, const std::function<void()>& function, unsigned int i)
	:
	Menu(parent, text)
{
	m_Id = i;
	m_ClickDelegate = std::make_unique<Event<>>(function);
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
	auto m = std::make_shared<MenuItem>(this, text, function, m_CurrentIndex++);
	m->Bind();
	m_MenuItems.push_back(m);
	return *m;
}

MenuItem& Menu::AddItem(const std::string& text)
{
	return AddItem(text, nullptr);
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