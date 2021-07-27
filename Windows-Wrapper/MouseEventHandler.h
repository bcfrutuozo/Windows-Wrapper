#pragma once

#include "Event.h"
#include "MouseEventArgs.h"

class Control;

class MouseEventHandler : public Event<Control*, MouseEventArgs*>
{
public:

	MouseEventHandler(const std::string& name, const std::function<void(Control*, MouseEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};