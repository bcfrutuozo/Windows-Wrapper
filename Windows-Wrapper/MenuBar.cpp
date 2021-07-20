#include "MenuBar.h"
#include "MenuItem.h"

MenuBar::MenuBar(Control* parent)
	:
	Menu(parent, "", 0, "")
{
	Handle = CreateMenu();
};

MenuBar::~MenuBar()
{
	
};

MenuItem& MenuBar::AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath)
{
	auto m = std::make_shared<MenuItem>(this, text, function, m_CurrentIndex++, m_MenuItems.size(), iconPath);
	m->Bind();
	m_MenuItems.push_back(m);
	return dynamic_cast<MenuItem&>(*m_MenuItems.back());
}

MenuItem& MenuBar::AddItem(const std::string& text, const std::string& iconPath)
{
	return AddItem(text, nullptr, iconPath);
}

void MenuBar::SetText(const std::string& text)
{
	Text = text;
}