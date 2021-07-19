#include "MenuBar.h"

MenuBar::MenuBar(Control* parent)
	:
	Menu(parent, "", 0, "")
{
	Handle = CreateMenu();
};

//Menu& MenuBar::AddMenu(const std::string& text)
//{
//	auto m = std::make_shared<Menu>(this, text, static_cast<unsigned int>(m_MenuItems.size()), ""); // Menubar doesn't have icons
//	m->Bind();
//	m_MenuItems.push_back(m);
//	return *m_MenuItems.back();
//}


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