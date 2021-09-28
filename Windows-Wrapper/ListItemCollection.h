#pragma once

#include "ListItem.h"

class ListControl;

class ListItemCollection : public std::vector<ListItem>
{
private:

	ListControl* Owner;

public:

	ListItemCollection(ListControl* owner);
	ListItemCollection(ListControl* owner, ListItemCollection& value);
	ListItemCollection(ListControl* owner, ListItem value[]);
	virtual ~ListItemCollection();
	
	ListControl* GetOwner() const noexcept;
};