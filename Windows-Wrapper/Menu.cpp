#include "Menu.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"

#include <filesystem>

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 1;

Menu::Menu(Control* parent, unsigned int subitemIndex, int section)
	:
	Menu(parent, "", subitemIndex, section)

{

};

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_Id(0),			// No processing for default menu.
	m_Section(section)
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

Menu& Menu::AddMenu(const std::string& text)
{
	// New Menu doesn't have icons and Section index is always 0.
	return Create<Menu>(this, text, static_cast<unsigned int>(Controls.size()), 0);
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

void Menu::Bind() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mi.wID = m_Id;
	mi.hSubMenu = (HMENU)Handle.ToPointer();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}

MenuItem& Menu::AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath)
{
	return Create<MenuItem>(this, text, function, m_CurrentIndex++, Controls.size(), m_Section, iconPath);
}

MenuItem& Menu::AddItem(const std::string& text, const std::string& iconPath)
{
	return AddItem(text, nullptr, iconPath);
}

MenuItem& Menu::AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked)
{
	return Create<MenuCheckItem>(this, text, function, m_CurrentIndex++, Controls.size(), m_Section, isChecked);
}

MenuItem& Menu::AddCheckItem(const std::string& text, bool isChecked)
{
	return AddCheckItem(text, nullptr, isChecked);
}

MenuItem& Menu::AddRadioItem(const std::string& text, const std::function<void()>& function, bool isSelected)
{
	return Create<MenuRadioItem>(this, text, function, m_CurrentIndex++, Controls.size(), m_Section, isSelected);
}

MenuItem& Menu::AddRadioItem(const std::string& text, bool isSelected)
{
	return AddRadioItem(text, nullptr, isSelected);
}

void Menu::AddSeparator()
{
	Create<MenuSeparator>(this, Controls.size());
	m_Section++;	// Increment section for check/radio button option sections
}