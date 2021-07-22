#include "MenuRadioItem.h"

MenuRadioItem::MenuRadioItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, int section, bool isSelected)
	:
	MenuItem(parent, text, function, i, subitemIndex, section),
	IsSelected(isSelected)
{

}

void MenuRadioItem::Bind() noexcept
{
	// Check if there's already a default set
	int begin = 0;
	int end = 0;
	
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_FTYPE;
	mi.fState = IsSelected ? MF_CHECKED : MF_UNCHECKED;
	mi.fType = MFT_RADIOCHECK;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);

	// Remove selection of other radio buttons on same submenu if any of them were create with IsSelected = TRUE
	if (IsSelected)
	{
		auto idx = InvalidateSection(m_Section);
		CheckMenuRadioItem(static_cast<HMENU>(Parent->Handle.ToPointer()), std::get<0>(idx), std::get<1>(idx), m_SubItemIndex, MF_BYPOSITION);
	}
}

void MenuRadioItem::DispatchEvent(unsigned int id)
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
		IsSelected = !IsSelected;
		auto idx = InvalidateSection(m_Section);
		CheckMenuRadioItem(static_cast<HMENU>(Parent->Handle.ToPointer()), std::get<0>(idx), std::get<1>(idx), m_SubItemIndex, MF_BYPOSITION);
	}
}