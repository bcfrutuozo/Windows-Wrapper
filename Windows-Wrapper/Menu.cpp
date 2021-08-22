#include "Menu.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"
#include "Window.h"

Menu::Menu(Control* parent, unsigned int subitemIndex, int section)
	:
	Control(parent, ""),
	m_SubItemIndex(subitemIndex),
	m_Section(section)
{
	Handle = CreateMenu();
}

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_Section(section)
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	// If Menu entry is a submenu, it cannot be destroyed.
	if (Controls.size() > 0)
	{
		return;
	}

	if (DestroyMenu(static_cast<HMENU>(Handle.ToPointer())) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

void Menu::Update()
{
	for (const auto& c : Controls)
	{
		if (const auto& menu = dynamic_cast<Menu*>(c.get()))
		{
			menu->Update();
		}
	}
}
