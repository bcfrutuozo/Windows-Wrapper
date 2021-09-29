#include "ListItem.h"
#include "ListControl.h"

ListItem::ListItem(int id, const std::string& value)
	:
	Id(id),
	Value(value),
	Visible(true),
	Selected(false),
	Tabulated(false)
{

}