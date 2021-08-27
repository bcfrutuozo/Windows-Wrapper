#pragma once

#include "Event.h"
#include "EventArgs.h"

class EventHandler : public Event<EventArgs*>
{
public:

	EventHandler(const std::string& name, const std::function<void(Object*, EventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};