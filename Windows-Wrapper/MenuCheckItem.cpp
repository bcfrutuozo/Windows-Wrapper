//#include "MenuCheckItem.h"
//#include "EventHandler.h"
//
//MenuCheckItem::MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked)
//	:
//	MenuItem(parent, text, subitemIndex, section),
//	IsChecked(isChecked)
//{
//	Initialize();
//}
//
//void MenuCheckItem::Initialize()
//{
//	MENUITEMINFO mi = { 0 };
//	mi.cbSize = sizeof(MENUITEMINFO);
//	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
//	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
//	mi.wID = GetId();
//	mi.dwTypeData = const_cast<char*>(Text.c_str());
//	if (InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi) == 0)
//	{
//		throw CTL_LAST_EXCEPT();
//	}
//}
//
//void MenuCheckItem::Update()
//{
//	// Change MenuItemChange only if callback is executed
//	IsChecked = !IsChecked;
//	MENUITEMINFO mi = { 0 };
//	mi.cbSize = sizeof(MENUITEMINFO);
//	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
//	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
//	mi.wID = GetId();
//	mi.dwTypeData = const_cast<char*>(Text.c_str());
//	SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
//}