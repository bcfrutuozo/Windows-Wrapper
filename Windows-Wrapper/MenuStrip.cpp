#include "MenuStrip.h"
#include "MenuItem.h"

MenuStrip::MenuStrip(Control* parent)
	:
	Menu(parent, 0, 0)
{
	Handle = CreateMenu();
};

void MenuStrip::Bind() noexcept
{

}