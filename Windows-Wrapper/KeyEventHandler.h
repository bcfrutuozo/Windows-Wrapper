#pragma once

#include "Event.h"
#include "KeyEventArgs.h"

class KeyEventHandler : public Event<KeyEventArgs*>
{
public:

	KeyEventHandler(const std::string& name, const std::function<void(Object*, KeyEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};