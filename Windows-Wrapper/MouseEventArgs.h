#pragma once

#include "EventArgs.h"
#include "Mouse.h"

class MouseEventArgs : public EventArgs
{
public:

	MouseEventArgs(const Mouse* mouse)
		:
		m(mouse)
	{}

	const Mouse* m;
};