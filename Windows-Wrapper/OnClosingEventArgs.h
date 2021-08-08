#pragma once

#include "EventArgs.h"
#include "Enums.h"

class OnClosingEventArgs : public EventArgs
{
public:

	OnClosingEventArgs(const CloseReason& closeReason, bool& cancel)
		:
		m_CloseReason(closeReason),
        m_Cancel(cancel)
	{}

	CloseReason m_CloseReason;
    bool m_Cancel;
};