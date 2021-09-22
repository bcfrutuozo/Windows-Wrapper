#include "ListItem.h"
#include "ListControl.h"

ListItem::ListItem(const std::string& value)
	:
	Id(id),
	Value(value)
{

}

ListItem::ListItem(const ListItem& src)
{
	Id = src.Id;
	Value = src.Value;
	Visible = src.Visible;
	Selected = src.Selected;
}
