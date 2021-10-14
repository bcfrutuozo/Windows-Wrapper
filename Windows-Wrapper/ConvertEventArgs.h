#pragma once

#include "EventArgs.h"
#include "Type.h"

class ConvertEventArgs : public EventArgs
{
public:

	Object* Value;
	Type DesiredType;

	ConvertEventArgs(Object* object, Type desiredType)
		:
		Value(object),
		DesiredType(desiredType)
	{	}
};