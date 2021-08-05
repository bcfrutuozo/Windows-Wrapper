#pragma once

#include "EventArgs.h"
#include "Keyboard.h"

class KeyPressEventArgs : public EventArgs
{
public:

	char KeyChar;
	bool Handled;

	KeyPressEventArgs(const char keyChar)
		:
		KeyChar(keyChar),
		Handled(false)
	{}
};