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
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}