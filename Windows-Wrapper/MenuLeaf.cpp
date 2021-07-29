#include "MenuLeaf.h"
#include "MenuEntryIconItem.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"
#include "MenuSeparator.h"
#include "Window.h"

MenuLeaf::MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	MenuRoot(parent, text, subitemIndex, section)
{

}

void MenuLeaf::Disable() noexcept
{
	if (IsEnabled())
	{
		EnableMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_DISABLED);

		if (const auto wnd = GetWindow())
		{
			DrawMenuBar(static_cast<HWND>(wnd->Handle.ToPointer()));
		}

		Enabled = false;
	}
}

void MenuLeaf::Enable() noexcept
{
	if (!IsEnabled())
	{
		EnableMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_ENABLED);

		if (const auto wnd = GetWindow())
		{
			DrawMenuBar(static_cast<HWND>(wnd->Handle.ToPointer()));
		}

		Enabled = true;
	}
}

MenuItem& MenuLeaf::AddItemWithIcon(const std::string& text, const std::string& iconPath)
{
	return Create<MenuEntryIconItem>(this, text, Controls.size(), m_Section, iconPath);
}

MenuItem& MenuLeaf::AddCheckItem(const std::string& text, bool isChecked)
{
	return Create<MenuCheckItem>(this, text, Controls.size(), m_Section, isChecked);
}

MenuItem& MenuLeaf::AddRadioItem(const std::string& text, bool isSelected)
{
	return Create<MenuRadioItem>(this, text, Controls.size(), m_Section, isSelected);
}

void MenuLeaf::AddSeparator()
{
	Create<MenuSeparator>(this, Controls.size());
	++m_Section;	// Increment section for check/radio button option sections
}