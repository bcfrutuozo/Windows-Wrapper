#include "ListItemCollection.h"
#include "ListControl.h"

ListItemCollection::ListItemCollection(ListControl* owner)
	:
	Collection(owner)
{

}

ListItemCollection::ListItemCollection(ListControl* owner, ListItemCollection& value)
	:
	Collection(owner)
{
	for (const auto& item : value)
	{
		Add(item);
	}
}

ListItemCollection::ListItemCollection(ListControl* owner, ListItem* value[])
	:
	Collection(owner)
{
	while (value != nullptr)
	{
		Add(*value);
		++value;
	}
}