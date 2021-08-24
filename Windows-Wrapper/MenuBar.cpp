//#include "MenuBar.h"
//#include "MenuRoot.h"
//
//void MenuBar::Initialize()
//{
//	MENUITEMINFO mi = { 0 };
//	mi.cbSize = sizeof(MENUITEMINFO);
//	mi.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
//	mi.wID = GetId();
//	mi.hSubMenu = (HMENU)Handle.ToPointer();
//	mi.dwTypeData = const_cast<char*>(Text.c_str());
//	if (InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi) == 0)
//	{
//		throw CTL_LAST_EXCEPT();
//	}
//}
//
//MenuBar::MenuBar(Control* parent)
//	:
//	MenuRoot(parent, 0, 0)
//{
//
//};
//
//void MenuBar::Hide() noexcept
//{
//	if (IsShown())
//	{
//		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), NULL) == 0)
//		{
//			throw CTL_LAST_EXCEPT();
//		};
//
//		IsVisible = false;
//	}
//}
//
//void MenuBar::Show() noexcept
//{
//	if (!IsShown())
//	{
//		if (SetMenu(static_cast<HWND>(Parent->Handle.ToPointer()), static_cast<HMENU>(Handle.ToPointer())) == 0)
//		{
//			throw CTL_LAST_EXCEPT();
//		};
//
//		IsVisible = true;
//	}
//}