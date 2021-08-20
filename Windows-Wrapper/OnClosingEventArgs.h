#pragma once

#include "EventArgs.h"
#include "Enums.h"

class OnClosingEventArgs : public EventArgs
{
public:

	OnClosingEventArgs(const CloseReason closeReason, bool cancel)
		:
		CloseReason(closeReason),
        Cancel(cancel)
	{}

	CloseReason CloseReason;
    bool Cancel;
};