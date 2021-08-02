#pragma once

#include "EventArgs.h"
#include "Keyboard.h"

class KeyEventArgs : public EventArgs
{
public:

	KeyEventArgs(const Keyboard* keyboard)
		:
		k(keyboard)
	{}

	const Keyboard* k;
};