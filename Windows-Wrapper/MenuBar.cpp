#include "MenuBar.h"

MenuBar::MenuBar(Control* parent)
	:
	Menu(parent, "")
{
	Handle = CreateMenu();
};

MenuBar::~MenuBar()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

Menu& MenuBar::AddMenu(const std::string& text)
{
	Menu* m = new Menu(this, text);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}


MenuItem& MenuBar::AddItem(const std::string& text, const std::function<void()>& function)
{
	MenuItem* m = new MenuItem(this, text, function, m_CurrentIndex++);
	m_MenuItems.emplace_back(m);
	m->Bind();
	return *m;
}

MenuItem& MenuBar::AddItem(const std::string& text)
{
	return AddItem(text, nullptr);
}

void MenuBar::SetText(const std::string& text)
{
	Text = text;
}
// 
// 
//MenuBar::MenuBar()
//	:
//	Control(nullptr)
//{
//	Handle = CreateMenu();
//}
//
//MenuBar::MenuBar(Control* parent)
//	:
//	Control(parent, "")
//{
//	Handle = CreateMenu();
//};
//
//MenuBar::~MenuBar()
//{
//	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
//}
//
//Menu& MenuBar::AddMenu(const std::string& text)
//{
//	Menu* m = new Menu(this, text);
//	m_MenuItems.emplace_back(m);
//	m->Bind();
//	return *m;
//}
//
//
//MenuItem& MenuBar::AddItem(const std::string& text, const std::function<void()>& function)
//{
//	MenuItem* m = new MenuItem(this, text, function);
//	m_MenuItems.emplace_back(m);
//	m->Bind();
//	return *m;
//}
//
//MenuItem& MenuBar::AddItem(const std::string& text)
//{
//	AddItem(text, nullptr);
//}
//
//void MenuBar::SetText(const std::string& text)
//{
//	Text = text;
//}