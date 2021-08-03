#include "MenuEntryItem.h"

MenuEntryItem::MenuEntryItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	MenuItem(parent, text, subitemIndex, section)
{
	Initialize();
}

void MenuEntryItem::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID;
	mi.wID = GetId();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}