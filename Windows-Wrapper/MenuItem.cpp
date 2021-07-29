#include "MenuItem.h"
#include "Window.h"

MenuItem::MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, section)
{

}

MenuItem::~MenuItem()
{

}

void MenuItem::Disable() noexcept
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

void MenuItem::Enable() noexcept
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