#pragma once

#include "EventArgs.h"

class IEvent : public Object
{
public:

	virtual const std::string& GetName() const noexcept = 0;
	virtual void Trigger(Object* sender, EventArgs* e) const noexcept = 0;
};