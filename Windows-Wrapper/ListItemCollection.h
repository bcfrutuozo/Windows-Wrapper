#pragma once

#include "Collection.h"
#include "ListItem.h"

class ListControl;

class ListItemCollection : public Collection<ListItem>
{
public:

	ListItemCollection(ListControl* owner);
	ListItemCollection(ListControl* owner, ListItemCollection& value);
	ListItemCollection(ListControl* owner, ListItem* value[]);
};