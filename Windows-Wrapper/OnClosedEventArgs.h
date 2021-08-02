#pragma once

#include "EventArgs.h"
#include "Mouse.h"
#include "Enums.h"

class OnClosedEventArgs : public EventArgs
{
public:

	OnClosedEventArgs(const CloseReason& closeReason)
		:
		m_CloseReason(closeReason)
	{}

	CloseReason m_CloseReason;
};