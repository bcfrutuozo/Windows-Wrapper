#include "ListItem.h"
#include "ListControl.h"

ListItem::ListItem(const std::string& value)
	:
	Value(value),
	Visible(true),
	Selected(false)
{

}

ListItem::ListItem(const ListItem& src)
{
	Value = src.Value;
	Visible = src.Visible;
	Selected = src.Selected;
}
