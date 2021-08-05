#pragma once

#include "EventArgs.h"
#include "Keyboard.h"

class CancelEventArgs : public EventArgs
{
public:

	CancelEventArgs()
		:
		CancelEventArgs(false)
	{}

	CancelEventArgs(bool cancel)
		:
		Cancel(cancel)
	{}

	bool Cancel;
};