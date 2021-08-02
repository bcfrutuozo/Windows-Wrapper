#pragma once

#include "Event.h"
#include "OnClosedEventArgs.h"

class Control;

class OnClosedEventHandler : public Event<Control*, OnClosedEventArgs*>
{
public:

	OnClosedEventHandler(const std::string& name, const std::function<void(Control*, OnClosedEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};