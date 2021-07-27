#include "Control.h"
#include "EventHandler.h"
#include "MouseEventHandler.h"

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
	// TODO: Register all default events
	Events.Register(new EventHandler("OnClick", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnClick(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseClick", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseClick(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseDoubleClick", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseDoubleClick(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseDown", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseDown(c, e); })));
	Events.Register(new EventHandler("OnMouseEnter", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseEnter(c, e); })));
	Events.Register(new EventHandler("OnMouseHover", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseHover(c, e); })));
	Events.Register(new EventHandler("OnMouseLeave", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseLeave(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseMove", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseMove(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseUp", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseUp(c, e); })));
	Events.Register(new MouseEventHandler("OnMouseWheel", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseWheel(c, e); })));
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