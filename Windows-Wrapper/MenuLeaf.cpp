#include "MenuLeaf.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"
#include "MenuSeparator.h"

MenuLeaf::MenuLeaf(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	MenuRoot(parent, text, subitemIndex, section)
{

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