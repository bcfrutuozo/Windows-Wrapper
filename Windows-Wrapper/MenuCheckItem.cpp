#include "MenuCheckItem.h"

MenuCheckItem::MenuCheckItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked)
	:
	MenuItem(parent, text, subitemIndex, section)
{

}

void MenuCheckItem::Bind() noexcept
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}

// Dispatch is overridden to process the Check on the item after the callback
void MenuCheckItem::Dispatch(const std::string& name)
{
	try
	{
		Control::Dispatch(name);
	}
	catch (...)
	{
		// Throw exception during processing and don't change MenuChangeItem behavior
		throw;
	}

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