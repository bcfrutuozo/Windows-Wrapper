#include "Control.h"

Control::Control()
	:
	Size(0, 0),
	Location(0, 0)
{

}

Control::Control(Control* parent, const std::string& text)
	:
	Parent(parent),
	Text(text),
	Size(0, 0),
	Location(0, 0)
{

}

Control::Control(Control* parent, const std::string& text, int width, int height, int x, int y)
	:
	Parent(parent),
	Text(text),
	Size(width, height),
	Location(x, y)
{

}

Control::Control(const std::string& text)
	:
	Text(text),
	Size(0, 0),
	Location(0, 0)
{

}

Control::Control(const std::string& text, int width, int height, int x, int y)
	:
	Text(text),
	Size(width, height),
	Location(x, y)
{

}


Control::~Control()
{

}