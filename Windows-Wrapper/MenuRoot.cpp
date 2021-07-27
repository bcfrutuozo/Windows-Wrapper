#include "MenuRoot.h"
#include "MenuLeaf.h"
#include "MenuItem.h"

MenuRoot::MenuRoot(Control* parent, unsigned int subitemIndex, int section)
	:
	Menu(parent, subitemIndex, section)
{
	Initialize();
}

MenuRoot::MenuRoot(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	Menu(parent, text, subitemIndex, section)
{
	Initialize();
}

void MenuRoot::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mi.wID = m_Id;
	mi.hSubMenu = (HMENU)Handle.ToPointer();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}

MenuLeaf& MenuRoot::AddMenu(const std::string& text)
{
	// New Menu doesn't have icons and Section index is always 0.
	return Create<MenuLeaf>(this, text, static_cast<unsigned int>(Controls.size()), 0);
}

MenuItem& MenuRoot::AddItem(const std::string& text, const std::string& iconPath)
{
	return Create<MenuItem>(this, text, Controls.size(), m_Section, iconPath);
}