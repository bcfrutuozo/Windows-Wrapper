#pragma once

#include "EventArgs.h"
#include "Graphics.h"
#include "Rectangle.h"
#include "IDeviceContext.h"

class PaintEventArgs : public EventArgs, IDisposable
{
public:

	PaintEventArgs(Graphics* const graphics, Drawing::Rectangle rect)
		:
		Graphics(graphics),
		Rectangle(rect)
	{

	}

	void Dispose() override
	{
		Graphics->Dispose();
	}

	Graphics* const Graphics;
	Drawing::Rectangle Rectangle;
};