#include "MenuBar.h"

MenuBar::MenuBar(Control* parent)
	:
	Menu(parent, "")
{
	Handle = CreateMenu();
};

Menu& MenuBar::AddMenu(const std::string& text)
{
	auto m = std::make_shared<Menu>(this, text);
	m->Bind();
	m_MenuItems.push_back(m);
	return *m_MenuItems.back();
}


MenuItem& MenuBar::AddItem(const std::string& text, const std::function<void()>& function)
{
	auto m = std::make_shared<MenuItem>(this, text, function, m_CurrentIndex++);
	m->Bind();
	m_MenuItems.push_back(m);
	return dynamic_cast<MenuItem&>(*m_MenuItems.back());
}

MenuItem& MenuBar::AddItem(const std::string& text)
{
	return AddItem(text, nullptr);
}

void MenuBar::SetText(const std::string& text)
{
	Text = text;
}