#pragma once

#include "Event.h"
#include "PaintEventArgs.h"

class PaintEventHandler : public Event<PaintEventArgs*>
{
public:

	PaintEventHandler(const std::string& name, const std::function<void(Object*, PaintEventArgs*)>& callback)
		:
		Event(name, callback)
	{

	}
};