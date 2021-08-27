#pragma once

#include "Event.h"
#include "CancelEventArgs.h"

class Control;

class CancelEventHandler : public Event<CancelEventArgs*>
{
public:

	CancelEventHandler(const std::string& name, const std::function<void(Object*, CancelEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};