#pragma once

#include "Event.h"
#include "OnClosingEventArgs.h"

class OnClosingEventHandler : public Event<OnClosingEventArgs*>
{
public:

	OnClosingEventHandler(const std::string& name, const std::function<void(Object*, OnClosingEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};