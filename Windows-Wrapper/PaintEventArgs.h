#pragma once

#include "EventArgs.h"
#include "Graphics.h"
#include "Rectangle.h"
#include "IDeviceContext.h"

class PaintEventArgs : public EventArgs, public IDeviceContext
{
public:

	PaintEventArgs(Graphics gfx, Drawing::Rectangle rect)
		:
		IDeviceContext(gfx.GetHDC()),
		Graphics(gfx),
		Rectangle(rect)
	{

	}

	void Dispose() override
	{
		Graphics.Dispose();
	}

	Graphics Graphics;
	Drawing::Rectangle Rectangle;
};