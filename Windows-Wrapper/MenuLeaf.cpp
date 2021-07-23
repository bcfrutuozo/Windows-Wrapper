#include "MenuLeaf.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"
#include "MenuSeparator.h"

MenuLeaf::MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	MenuRoot(parent, text, subitemIndex, section)
{

}

MenuItem& MenuLeaf::AddCheckItem(const std::string& text, const std::function<void()>& function, bool isChecked)
{
	return Create<MenuCheckItem>(this, text, function, m_CurrentIndex++, Controls.size(), m_Section, isChecked);
}

MenuItem& MenuLeaf::AddCheckItem(const std::string& text, bool isChecked)
{
	return AddCheckItem(text, nullptr, isChecked);
}

MenuItem& MenuLeaf::AddRadioItem(const std::string& text, const std::function<void()>& function, bool isSelected)
{
	return Create<MenuRadioItem>(this, text, function, m_CurrentIndex++, Controls.size(), m_Section, isSelected);
}

MenuItem& MenuLeaf::AddRadioItem(const std::string& text, bool isSelected)
{
	return AddRadioItem(text, nullptr, isSelected);
}

void MenuLeaf::AddSeparator()
{
	Create<MenuSeparator>(this, Controls.size());
	m_Section++;	// Increment section for check/radio button option sections
}