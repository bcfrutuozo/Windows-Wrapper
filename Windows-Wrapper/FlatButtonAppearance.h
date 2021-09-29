#pragma once

#include "CommonObject.h"

class FlatButtonAppearance : public Object
{
public:

	// Gets or sets the color of the border around the button.
	Color BorderColor = Color::Border();

	// Gets or sets a value that specifies the size, in pixels, of the border around the button.
	int BorderSize = 1;

	// Gets or sets the color of the client area of the button when the button is checked and the mouse pointer is outside the bounds of the control.
	Color CheckedBackColor = Color::Default();

	// Gets or sets the color of the client area of the button when the mouse is pressed within the bounds of the control.
	Color MouseDownBackColor = Color(204, 228, 247, 255);

	// Gets or sets the color of the client area of the button when the mouse pointer is within the bounds of the control.
	Color MouseOverBackColor = Color(229, 241, 251, 255);
};