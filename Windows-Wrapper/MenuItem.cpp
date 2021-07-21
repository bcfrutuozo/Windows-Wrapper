#include "MenuItem.h"

MenuItem::MenuItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, iconPath)
{
	m_Id = i;
	m_ClickDelegate = std::make_unique<Event<>>(function);
}

MenuItem::~MenuItem()
{
	if (m_Icon != nullptr)
	{
		DeleteObject(static_cast<HGDIOBJ>(m_Icon));
	}
}

void MenuItem::Bind()
{
	if (!m_IconPath.empty())
	{
		m_Icon = static_cast<HBITMAP>(LoadImage(NULL, m_IconPath.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
		InsertMenu(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, MF_BYPOSITION | MF_POPUP, m_Id, Text.c_str());
		MENUITEMINFO mi = { 0 };
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_BITMAP;
		mi.hbmpItem = m_Icon;
		SetMenuItemInfo(static_cast<HMENU>(Parent->Handle.ToPointer()), m_SubItemIndex, true, &mi);
	}
	else
	{
		MENUITEMINFO mi = { 0 };
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_STRING | MIIM_ID;
		mi.wID = m_Id;
		mi.dwTypeData = const_cast<char*>(Text.c_str());
		InsertMenuItem(static_cast<HMENU>(Parent->Handle.ToPointer()), Handle.ToInt32(), FALSE, &mi);
	}
}

void MenuItem::SetText(const std::string& text)
{
	Text = text;
}

void MenuItem::OnClickSet(const std::function<void()>& function)
{
	m_ClickDelegate = std::make_unique<Event<>>(function);
}

void MenuItem::OnClick() const
{
	if (m_ClickDelegate != nullptr)
	{
		m_ClickDelegate->Trigger();
	}
};