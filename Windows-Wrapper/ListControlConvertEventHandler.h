#pragma once

#include "Event.h"
#include "ListControlConvertEventArgs.h"

class ListControlConvertEventHandler : public Event<ListControlConvertEventArgs*>
{
public:

	ListControlConvertEventHandler(const std::string& name, const std::function<void(Object*, ListControlConvertEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};