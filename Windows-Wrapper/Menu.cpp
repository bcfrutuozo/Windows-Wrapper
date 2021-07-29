#include "Menu.h"
#include "MenuSeparator.h"
#include "MenuItem.h"
#include "MenuCheckItem.h"
#include "MenuRadioItem.h"
#include "Window.h"

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Menu::m_CurrentIndex = 0;

Menu::Menu(Control* parent, unsigned int subitemIndex, int section)
	:
	Control(parent, ""),
	m_SubItemIndex(subitemIndex),
	m_Id(m_CurrentIndex++),
	m_Section(section)
{
	Handle = CreateMenu();
}

Menu::Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section)
	:
	Control(parent, text),
	m_SubItemIndex(subitemIndex),
	m_Id(m_CurrentIndex++),			// No processing for default menu.
	m_Section(section)
{
	Handle = CreatePopupMenu();
};

Menu::~Menu()
{
	DestroyMenu(static_cast<HMENU>(Handle.ToPointer()));
}

Menu* Menu::GetById(unsigned int id) noexcept
{
	if (m_Id == id)
	{
		return this;
	}

	for (const auto& c : Controls)
	{
		auto ret = dynamic_cast<Menu*>(c.get())->GetById(id);
		if (ret != nullptr)
		{
			return ret;
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

Window* Menu::GetWindow() const noexcept
{
	if (Parent != nullptr)
	{
		if (Parent->GetType() == typeid(Window))
		{
			return dynamic_cast<Window*>(Parent);
		}

		if (const auto c = dynamic_cast<Menu*>(Parent))
		{
			return c->GetWindow();
		}
	}

	return nullptr;
}