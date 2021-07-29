#include "MenuItem.h"

#include <filesystem>

MenuItem::MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, section)
{
	
}

MenuItem::~MenuItem()
{
	
}

void MenuItem::Enable() noexcept
{
	if (!Enabled)
	{
		EnableMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_ENABLED);
		Enabled = true;
		Grayed = false;
	}
}

void MenuItem::Disable() noexcept
{
	if (Enabled || !Grayed)
	{
		EnableMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_DISABLED);
		Enabled = false;
		Grayed = false;
	}
}

void MenuItem::Gray() noexcept
{
	if (!Grayed)
	{
		EnableMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_GRAYED);
		Enabled = false;
		Grayed = true;
	}
}