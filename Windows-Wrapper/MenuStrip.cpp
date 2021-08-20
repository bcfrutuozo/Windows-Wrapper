#include "MenuStrip.h"
#include "MenuRoot.h"

void MenuStrip::Initialize()
{
	MENUITEMINFO mi = { 0 };
	mi.cbSize = sizeof(MENUITEMINFO);
	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
	mi.wID = GetId();
	mi.hSubMenu = (HMENU)Handle.ToPointer();
	mi.dwTypeData = const_cast<char*>(Text.c_str());
	if (InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

MenuStrip::MenuStrip(Control* parent)
	:
	MenuRoot(parent, 0, 0)
{

};

void MenuStrip::Hide() noexcept
{
	if (IsShown())
	{
		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), NULL) == 0)
		{
			throw CTL_LAST_EXCEPT();
		};

		IsVisible = false;
	}
}

void MenuStrip::Show() noexcept
{
	if (!IsShown())
	{
		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), static_cast<HMENU>(Handle.ToPointer())) == 0)
		{
			throw CTL_LAST_EXCEPT();
		};

		IsVisible = true;
	}
}