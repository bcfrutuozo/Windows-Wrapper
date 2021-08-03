#pragma once

#include "EventArgs.h"
#include "Keyboard.h"

class KeyPressEventArgs : public EventArgs
{
public:

	KeyPressEventArgs(const char keyChar)
		:
		KeyChar(keyChar)
	{}

	char KeyChar;
};