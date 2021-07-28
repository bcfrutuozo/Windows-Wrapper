#include "MenuCheckItem.h"
#include "EventHandler.h"

void MenuCheckItem::OnInternalUpdate(Control* const sender, EventArgs* const args)
{
	// Change MenuItemChange only if callback is executed
	IsChecked = !IsChecked;
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}

MenuCheckItem::MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked)
	:
	MenuItem(parent, text, subitemIndex, section)
{
	Initialize();

	// The OnInternalUpdate() event could be registered in Control class. However, it's registered on each child class who implement
	// the function to avoid unnecessary calls for the other classes
	Events.Register(new EventHandler("OnInternalUpdate", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnInternalUpdate(c, e); })));
}

void MenuCheckItem::Initialize() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}