#pragma once

#include "ConvertEventArgs.h"

class ListControlConvertEventArgs : public ConvertEventArgs
{
public:

	Object* ListItem;

	ListControlConvertEventArgs(Object* object, Type desiredType, Object* listItem)
		:
		ConvertEventArgs(object, desiredType),
		ListItem(listItem)
	{	}
};