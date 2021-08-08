#pragma once

#include "EventArgs.h"

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