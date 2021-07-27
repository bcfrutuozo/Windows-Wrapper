#include "MenuRadioItem.h"

MenuRadioItem::MenuRadioItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked)
	:
	MenuItem(parent, text, subitemIndex, section),
	IsChecked(isChecked)
{
	Initialize();
}

void MenuRadioItem::Initialize() noexcept
{
	// Check if there's already a default set
	int begin = 0;
	int end = 0;

	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_FTYPE;
	mi.fState = IsChecked ? MF_CHECKED : MF_UNCHECKED;
	mi.fType = MFT_RADIOCHECK;
	mi.wID = m_Id;
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
	
	// Remove selection of other radio buttons on same submenu if any of them were create with IsSelected = TRUE
	if (IsChecked)
	{
		auto idx = InvalidateSection(m_Section);
		CheckMenuRadioItem(static_cast<HMENU>(Parent->Handle.ToPointer()), std::get<0>(idx), std::get<1>(idx), m_SubItemIndex, MF_BYPOSITION);
	}
}

// Dispatch is overridden to process the Radio submenu check after the callback
void MenuRadioItem::Dispatch(const std::string& name)
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
	auto idx = InvalidateSection(m_Section);
	CheckMenuRadioItem(static_cast<HMENU>(Parent->Handle.ToPointer()), std::get<0>(idx), std::get<1>(idx), m_SubItemIndex, MF_BYPOSITION);
}

std::tuple<int, int> MenuRadioItem::InvalidateSection(int section)
{
	int begin = 0;
	int end = 0;

	// This is horrible for now. But it's working!
	// Optimizations come later! :D
	for (const auto& obj : Parent->Controls)
	{
		auto c = dynamic_cast<Menu*>(obj.get());

		if (obj == nullptr)	continue;

		// If section is not the desired one, skip until find
		if (c->m_Section < section)	continue;

		// If the found section is higher, break processing.
		if (c->m_Section > section)	break;

		if (c->m_Section == section)
		{
			// Get the submenu indices
			if (begin == 0)
			{
				begin = c->m_SubItemIndex;
				end = begin;
			}
			else
			{
				end++; // Increase section index until we get the final index 
			}

			if (c->m_SubItemIndex != m_SubItemIndex)
			{
				if (c->GetType() == typeid(MenuRadioItem))
				{
					auto obj = dynamic_cast<MenuRadioItem*>(c);
					obj->IsChecked = false;
				}
			}
		}
	}

	return std::tuple(begin, end);
}