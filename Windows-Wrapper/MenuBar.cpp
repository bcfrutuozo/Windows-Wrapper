#include "MenuBar.h"
#include "MenuItem.h"

MenuBar::MenuBar(Control* parent)
	:
	Menu(parent, "", 0, 0, "")
{
	Handle = CreateMenu();
};

MenuBar::~MenuBar()
{
	
};