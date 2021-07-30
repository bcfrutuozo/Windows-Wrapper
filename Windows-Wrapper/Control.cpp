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
	//OnClickSet(std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnClick(c, e); }));
	//Events.Register(std::make_unique<EventHandler>("OnClick", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnClick(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseClick", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseClick(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseDoubleClick", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseDoubleClick(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseDown", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseDown(c, e); })));
	//Events.Register(std::make_unique<EventHandler>("OnMouseEnter", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseEnter(c, e); })));
	//Events.Register(std::make_unique<EventHandler>("OnMouseHover", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseHover(c, e); })));
	//Events.Register(std::make_unique<EventHandler>("OnMouseLeave", std::function<void(Control* c, EventArgs* e)>([this](Control* c, EventArgs* e) { OnMouseLeave(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseMove", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseMove(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseUp", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseUp(c, e); })));
	//Events.Register(std::make_unique<MouseEventHandler>("OnMouseWheel", std::function<void(Control* c, MouseEventArgs* e)>([this](Control* c, MouseEventArgs* e) { OnMouseWheel(c, e); })));
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

void Control::Delete()
{
	// Recursive delete is necessary to avoid a parent destruction prior to children
	// This could cause some WinAPI errors during some controls deletion
	for (const auto& c : Controls)
	{
		c->Delete();
	}
	
	Parent = nullptr;
	Controls.clear();
	Events.Clear();
	Controls.shrink_to_fit();
}

void Control::OnClickSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnClick", callback));
}

void Control::OnMouseClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseClick", callback));
}

void Control::OnMouseDoubleClick(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseDoubleClick", callback));
}

void Control::OnMouseDown(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseDown", callback));
}

void Control::OnMouseEnter(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseEnter", callback));
}

void Control::OnMouseHover(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseHover", callback));
}

void Control::OnMouseLeave(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseLeave", callback));
}

void Control::OnMouseMove(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseMove", callback));
}

void Control::OnMouseUp(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseUp", callback));
}

void Control::OnMouseWheel(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseWheel", callback));
}

const std::string& Control::GetText() const noexcept
{
	return Text;
}

// Exceptions
const std::string& Control::ControlException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMessageBuffer = nullptr;

	// Windows will allocate memory for error string and make our pointer point to it
	const DWORD nMessageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMessageBuffer),
		0,
		nullptr);

	// String length as 0 indicates a failure
	if (nMessageLength == 0)
	{
		return "Unidentified error code";
	}

	// Copy error string from windows allocated buffer to string
	std::string errorString = pMessageBuffer;

	// Free windows buffer
	LocalFree(pMessageBuffer);

	return errorString;
}

Control::HRException::HRException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{ }

const char* Control::HRException::what() const noexcept
{
	std::ostringstream oss;

	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
		<< "Description: " << GetErrorDescription() << std::endl
		<< GetErrorSpot();
	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const char* Control::HRException::GetType() const noexcept
{
	return "Window Exception";
}

HRESULT Control::HRException::GetErrorCode() const noexcept
{
	return hr;
}

const std::string& Control::HRException::GetErrorDescription() const noexcept
{
	return ControlException::TranslateErrorCode(hr);
}
