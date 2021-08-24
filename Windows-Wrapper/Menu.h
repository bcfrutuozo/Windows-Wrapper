//#pragma once
//
//#include "Control.h"
//#include "Event.h"
//
//#include <memory>
//
//class MenuItem;
//
//class Menu : public Control
//{
//	friend class Window;
//	friend class MenuCheckItem;
//	friend class MenuRadioItem;
//
//protected:
//
//	// Indices to help track menu item location
//	unsigned int m_SubItemIndex;
//	int m_Section;
//
//	Menu(Control* parent, unsigned int subitemIndex, int section);								// Base constructor for MenuStrip (CreateMenu())
//	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, int section);		// Base constructor for other menu types (CreatePopupMenu())
//	virtual ~Menu();
//
//	virtual void Initialize() override = 0;
//
//public:
//
//	void Update() override;
//};