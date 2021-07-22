#include "Control.h"

Control::Control()
	:
	Control(nullptr, "")
{

}

Control::Control(Control* parent, const std::string& text)
	:
	Control(parent, text, 0, 0, 0, 0)
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
	Control(nullptr, text, 0, 0, 0, 0)
{

}

Control::Control(const std::string& text, int width, int height, int x, int y)
	:
	Control(nullptr, text, width, height, x, y)
{

}

Control::~Control()
{

}

const std::string& Control::GetText() const noexcept
{
	return Text;
}