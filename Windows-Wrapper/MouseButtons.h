#pragma once

enum class MouseButtons
{
	// The left mouse button was pressed.
	Left = 1048576,

	// The middle mouse button was pressed.
	Middle = 4194304,

	// No mouse button was pressed.
	None = 0,

	// The right mouse button was pressed.
	Right = 2097152,

	// The first XButton(XBUTTON1) on Microsoft IntelliMouse Explorer was pressed.
	XButton1 = 8388608,

	// The second XButton(XBUTTON2) on Microsoft IntelliMouse Explorer was pressed.
	XButton2 = 16777216
};

constexpr inline MouseButtons operator&(MouseButtons a, MouseButtons b)
{
	return static_cast<MouseButtons>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr inline MouseButtons operator|(MouseButtons a, MouseButtons b)
{
	return static_cast<MouseButtons>(static_cast<int>(a) | static_cast<int>(b));
}