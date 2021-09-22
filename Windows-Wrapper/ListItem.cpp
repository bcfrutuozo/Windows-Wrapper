#include "ListItem.h"
#include "ListControl.h"

ListItem::ListItem(uint32_t id, const std::string& value)
	:
	Id(id),
	Value(value),
	Visible(true),
	Selected(false)
{

}

ListItem::ListItem(const ListItem& src)
{
	Id = src.Id;
	Value = src.Value;
	Visible = src.Visible;
	Selected = src.Selected;
}