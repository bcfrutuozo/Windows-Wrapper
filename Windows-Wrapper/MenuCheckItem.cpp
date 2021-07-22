#include "MenuCheckItem.h"

MenuCheckItem::MenuCheckItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, bool isChecked)
	:
	MenuItem(parent, text, function, i, subitemIndex, section),
	IsChecked(isChecked)
{

}

MenuCheckItem::~MenuCheckItem()
{

}

void MenuCheckItem::Bind()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING;
	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
}

void MenuCheckItem::SetText(const std::string& text)
{
	Text = text;
}

void MenuCheckItem::DispatchEvent(unsigned int id)
{
	if (m_Id == id)
	{
		try
		{
			m_ClickDelegate->Trigger();
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
}