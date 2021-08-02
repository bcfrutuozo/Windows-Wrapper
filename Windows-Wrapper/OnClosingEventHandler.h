#pragma once

#include "Event.h"
#include "OnClosingEventArgs.h"

class Control;

class OnClosingEventHandler : public Event<Control*, OnClosingEventArgs*>
{
public:

	OnClosingEventHandler(const std::string& name, const std::function<void(Control*, OnClosingEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};