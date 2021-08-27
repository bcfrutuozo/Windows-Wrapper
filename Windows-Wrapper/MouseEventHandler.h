#pragma once

#include "Event.h"
#include "MouseEventArgs.h"

class MouseEventHandler : public Event<MouseEventArgs*>
{
public:

	MouseEventHandler(const std::string& name, const std::function<void(Object*, MouseEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};