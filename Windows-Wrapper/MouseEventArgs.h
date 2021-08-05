#pragma once

#include "EventArgs.h"
#include "MouseButtons.h"

class MouseEventArgs : public EventArgs
{
public:

	// Gets which mouse button was pressed.
	MouseButtons Button;

	// Gets the number of times the mouse button was pressedand released.
	int Clicks;

	// Gets a signed count of the number of detents the mouse wheel has rotated, multiplied by the WHEEL_DELTA constant.A detent is one notch of the mouse wheel.
	unsigned int Delta;

	// Gets the location of the mouse during the generating mouse event.
	Point Location;

	// Gets the x - coordinate of the mouse during the generating mouse event.
	int X;

	// Gets the y - coordinate of the mouse during the generating mouse event.
	int Y;

	MouseEventArgs(MouseButtons button, int clicks, unsigned int delta, int x, int y)
		:
		Button(button),
		Clicks(clicks),
		Delta(delta),
		Location(x, y),
		X(x),
		Y(y)
	{
	}
};