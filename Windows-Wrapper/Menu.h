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

	unsigned int m_SubItemIndex;
	int m_Section;

	// Fields for callback binding
	unsigned int m_Id;
	std::unique_ptr<Event<>> m_ClickDelegate;
	static unsigned int m_CurrentIndex;

	Menu(Control* parent);
	Menu(Control* parent, const std::string& text, unsigned int subItemIndex, int section);
	virtual ~Menu();

	std::tuple<int, int> InvalidateSection(int section);
	virtual void DispatchEvent(unsigned int id);

public:

	virtual void Bind() noexcept override = 0;
};