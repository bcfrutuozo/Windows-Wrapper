#pragma once

#include "Event.h"
#include "OnClosedEventArgs.h"

class OnClosedEventHandler : public Event<OnClosedEventArgs*>
{
public:

	OnClosedEventHandler(const std::string& name, const std::function<void(Object*, OnClosedEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};