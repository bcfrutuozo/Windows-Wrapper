#include "MenuItem.h"

#include <filesystem>

MenuItem::MenuItem(Menu* parent, const std::string& text, unsigned int subitemIndex, int section, const std::string& iconPath)
	:
	Menu(parent, text, subitemIndex, section)
{
	
}

MenuItem::~MenuItem()
{
	
}