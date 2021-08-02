#pragma once

#include "Event.h"
#include "KeyEventArgs.h"

class Control;

class KeyEventHandler : public Event<Control*, KeyEventArgs*>
{
public:

	KeyEventHandler(const std::string& name, const std::function<void(Control*, KeyEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};