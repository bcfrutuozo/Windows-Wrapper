#pragma once

#include "Event.h"
#include "KeyPressEventArgs.h"

class Control;

class KeyPressEventHandler : public Event<Control*, KeyPressEventArgs*>
{
public:

	KeyPressEventHandler(const std::string& name, const std::function<void(Control*, KeyPressEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};