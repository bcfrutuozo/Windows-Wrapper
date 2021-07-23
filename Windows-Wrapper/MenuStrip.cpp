#include "MenuStrip.h"
#include "MenuRoot.h"

MenuStrip::MenuStrip(Control* parent)
	:
	MenuRoot(parent, "", 0, 0)
{
	Handle = CreateMenu();
};

void MenuStrip::Bind() noexcept
{

}