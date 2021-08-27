#pragma once

#include "Object.h"
#include "EventArgs.h"

#include <string>

class IEvent
{
public:

	virtual const std::string& GetName() const noexcept = 0;
	virtual void Trigger(Object* sender, EventArgs* e) const noexcept = 0;
};