#pragma once

#include "Event.h"
#include "EventArgs.h"

class Control;

class EventHandler : public Event<Control*, EventArgs*>
{
public:

	EventHandler(const std::string& name, const std::function<void(Control*, EventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};