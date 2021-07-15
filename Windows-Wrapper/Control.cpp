#include "Control.h"

Control::Control()
	:
	Parent(nullptr),
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
	Parent(nullptr),
	Text(text),
	Size(0, 0),
	Location(0, 0)
{

}

Control::Control(const std::string& text, int width, int height, int x, int y)
	:
	Parent(nullptr),
	Text(text),
	Size(width, height),
	Location(x, y)
{

}


Control::~Control()
{

}