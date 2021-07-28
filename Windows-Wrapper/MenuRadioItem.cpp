#include "MenuRadioItem.h"
#include "EventHandler.h"

// Triggered after all event dispatch
void MenuRadioItem::OnInternalUpdate(Control* const sender, EventArgs* const args)
{
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

MenuRadioItem::MenuRadioItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, bool isChecked)
	:
	MenuItem(parent, text, subitemIndex, section),
	IsChecked(isChecked)
{
	Initialize();

	// The OnInternalUpdate() event could be registered in Control class. However, it's registered on each child class who implement
	// the function to avoid unnecessary calls for the other classes
	Events.Register(new EventHandler("OnInternalUpdate", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnInternalUpdate(c, e); })));
}

void MenuRadioItem::Initialize() noexcept
{
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