#include "ListItemCollection.h"
#include "ListControl.h"

ListItemCollection::ListItemCollection(ListControl* owner)
	:
	Owner(owner)
{

}

ListItemCollection::ListItemCollection(ListControl* owner, ListItemCollection& value)
	:
	Owner(owner)
{
	for (const auto& item : value)
	{
		push_back(item);
	}
}

ListItemCollection::ListItemCollection(ListControl* owner, ListItem value[])
	:
	Owner(owner)
{
	while (value != nullptr)
	{
		push_back(*value);
		++value;
	}
}

ListItemCollection::~ListItemCollection()
{

}

ListControl* ListItemCollection::GetOwner() const noexcept
{
	return Owner;
}
