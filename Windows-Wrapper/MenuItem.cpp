#include "MenuItem.h"

#include <filesystem>

MenuItem::MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, section)
{
	if (!iconPath.empty())
	{
		m_Icon = static_cast<HBITMAP>(LoadImage(NULL, iconPath.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	}

	Initialize();
}

MenuItem::~MenuItem()
{
	if (m_Icon != nullptr)
	{
		DeleteObject(static_cast<HGDIOBJ>(m_Icon));
	}
}

void MenuItem::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);

	if (m_Icon != nullptr)
	{	
		InsertMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_POPUP, m_Id, Text.c_str());
		mi.fMask = MIIM_BITMAP;
		mi.hbmpItem = m_Icon;
		SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
	}
	else
	{
		mi.fMask = MIIM_STRING | MIIM_ID;
		mi.wID = m_Id;
		mi.dwTypeData = const_cast<char*>(Text.c_str());
		InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
	}
}