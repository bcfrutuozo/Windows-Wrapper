#pragma once

#include "EventArgs.h"
#include "ScrollEventType.h"
#include "ScrollOrientation.h"

class ScrollEventArgs : public EventArgs
{
private:

	const ScrollEventType m_Type;
	int m_NewValue;
	ScrollOrientation m_Orientation;
	int m_OldValue;

public:

	ScrollEventArgs(ScrollEventType type, int newValue)
		:
		m_OldValue(-1),
		m_Type(type),
		m_NewValue(newValue)
	{}

	ScrollEventArgs(ScrollEventType type, int newValue, ScrollOrientation scroll)
		:
		m_OldValue(-1),
		m_Type(type),
		m_NewValue(newValue),
		m_Orientation(scroll)
	{}

	ScrollEventArgs(ScrollEventType type, int oldValue, int newValue, ScrollOrientation scroll)
		:
		m_OldValue(oldValue),
		m_Type(type),
		m_NewValue(newValue),
		m_Orientation(scroll)
	{}

	const ScrollOrientation ScrollOrientation() { return m_Orientation; }
	const ScrollEventType Type() { return m_Type; }
	const int NewValue() { return m_NewValue; }
	const int OldValue() { return m_OldValue; }
};