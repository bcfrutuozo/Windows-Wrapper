#pragma once

#include "Event.h"
#include "KeyPressEventArgs.h"

class KeyPressEventHandler : public Event<KeyPressEventArgs*>
{
public:

	KeyPressEventHandler(const std::string& name, const std::function<void(Object*, KeyPressEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};