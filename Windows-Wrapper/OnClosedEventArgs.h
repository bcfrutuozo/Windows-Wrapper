#pragma once

#include "EventArgs.h"
#include "Enums.h"

class OnClosedEventArgs : public EventArgs
{
public:

	OnClosedEventArgs(const CloseReason& closeReason)
		:
		CloseReason(closeReason)
	{}

	CloseReason CloseReason;
};