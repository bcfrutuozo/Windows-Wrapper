#pragma once

#include "Event.h"
#include "CancelEventArgs.h"

class Control;

class CancelEventHandler : public Event<Control*, CancelEventArgs*>
{
public:

	CancelEventHandler(const std::string& name, const std::function<void(Control*, CancelEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};