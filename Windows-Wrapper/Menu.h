#pragma once

#include "Control.h"
#include "Event.h"

#include <memory>

class MenuItem;

class Menu : public Control
{
	friend class Window;
	friend class MenuCheckItem;
	friend class MenuRadioItem;

protected:

	// Indices to help track menu item location
	unsigned int m_SubItemIndex;
	int m_Section;

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;

	Menu(Control* parent, unsigned int subitemIndex, int section);								// Base constructor for MenuStrip (CreateMenu())
	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section);		// Base constructor for other menu types (CreatePopupMenu())
	virtual ~Menu();

	virtual void Initialize() noexcept override = 0;
	Menu* GetById(unsigned int id) noexcept;			// Search the whole Menu tree recursively
};