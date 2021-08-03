#include "MenuEntryIconItem.h"

#include <filesystem>

MenuEntryIconItem::MenuEntryIconItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath)
	:
	MenuItem(parent, text, subitemIndex, section),
	m_Icon(static_cast<HBITMAP>(LoadImage(NULL, iconPath.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE | LR_CREATEDIBSECTION)))
{
	Initialize();
}

MenuEntryIconItem::~MenuEntryIconItem()
{
	DeleteObject(static_cast<HGDIOBJ>(m_Icon));
}

void MenuEntryIconItem::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	InsertMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_POPUP, GetId(), Text.c_str());
	mi.fMask = MIIM_BITMAP;
	mi.hbmpItem = m_Icon;
	SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}