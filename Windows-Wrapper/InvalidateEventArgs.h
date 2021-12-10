#pragma once

#include "EventArgs.h"
#include "Rectangle.h"

class InvalidateEventArgs : public EventArgs
{
public:

	const Drawing::Rectangle InvalidRect;

	InvalidateEventArgs(Drawing::Rectangle invalidRect)
		:
		InvalidRect(invalidRect)
	{}
};