#pragma once

#include "Common.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"

class Control : public IHandle
{
protected:

	std::string Text;
	Control* Parent;
	Size Size;
	Point Location;

public:

	Control();
	Control(Control* parent, const std::string& text);
	Control(Control* parent, const std::string& text, int width, int height, int x, int y);
	Control(const std::string& text);
	Control(const std::string& text, int width, int height, int x, int y);
	~Control();

	virtual void SetText(const std::string& title) = 0;
};

