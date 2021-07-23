#include "Menu.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

Menu::Menu(Control* parent)
	:
	Menu(parent, "", 0, 0)

{

}

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_Id(m_CurrentIndex++),			// No processing for default menu.
	m_Section(section)
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

std::tuple<int, int> Menu::InvalidateSection(int section)
{
	int begin = 0;
	int end = 0;

	// This is horrible for now. But it's working!
	// Optimizations come later! :D
	for (const auto& obj : Parent->Controls)
	{
		auto c = dynamic_cast<Menu*>(obj.get());

		if (obj == nullptr) continue;

		// If section is not the desired one, skip until find
		if (c->m_Section < section)
		{
			continue;
		}

		// If the found section is higher, break processing.
		if (c->m_Section > section)
		{
			break;
		}

		if (c->m_Section == section)
		{
			// Get the submenu indices
			if (begin == 0)
			{
				begin = c->m_SubItemIndex;
				end = begin;
			}
			else
			{
				end++; // Increase section index until we get the final index 
			}

			if (c->m_SubItemIndex != m_SubItemIndex)
			{
				if (c->GetType() == typeid(MenuRadioItem))
				{
					auto obj = dynamic_cast<MenuRadioItem*>(c);
					obj->IsSelected = false;
				}
				else if (c->GetType() == typeid(MenuCheckItem))
				{
					auto obj = dynamic_cast<MenuCheckItem*>(c);
					obj->IsChecked = false;
				}
			}
		}
	}

	return std::tuple(begin, end);
}


void Menu::DispatchEvent(unsigned int id)
{
	if (m_Id == id)
	{
		return m_ClickDelegate->Trigger();
	}

	for (auto& ch : Controls)
	{
		auto obj = dynamic_cast<Menu*>(ch.get());

		if (obj == nullptr) continue;

		if (obj->GetType() != typeid(MenuSeparator))
		{
			obj->DispatchEvent(id);
		}
	}
};