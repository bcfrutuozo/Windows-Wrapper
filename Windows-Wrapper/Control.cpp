#include "Control.h"
#include "Window.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "MouseEventHandler.h"
#include "KeyPressEventHandler.h"
#include "CancelEventHandler.h"

// Declare m_Index = 1 setting 0 as null function (NULL || nullptr)
unsigned int Control::m_CurrentIndex = 1;
MessageMapper Control::Mapper = MessageMapper();

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
	Location(x, y),
	m_Id(m_CurrentIndex++)
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
	DeleteObject(m_Brush);
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

Control* Control::GetById(unsigned int id) noexcept
{
	if (m_Id == id)
	{
		return this;
	}

	for (const auto& c : Controls)
	{
		Control* ret = c->GetById(id);
		if (ret != nullptr)
		{
			return ret;
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

Control* Control::GetByHandle(const IntPtr p) noexcept
{
	if (Handle == p)
	{
		return this;
	}

	for (const auto& c : Controls)
	{
		Control* ret = c->GetByHandle(p);
		if (ret != nullptr)
		{
			return ret;
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;

}

const unsigned int Control::GetId() const noexcept
{
	return m_Id;
}

void Control::OnActivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnActivate", callback));
}

void Control::OnClickSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnClick", callback));
}

void Control::OnDeactivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnDeactivate", callback));
}

void Control::OnGotFocusSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnGotFocus", callback));
}

void Control::OnLostFocusSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnLostFocus", callback));
}

void Control::OnKeyDownSet(const std::function<void(Control* const c, KeyEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<KeyEventHandler>("OnKeyDown", callback));
}

void Control::OnKeyPressSet(const std::function<void(Control* const c, KeyPressEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<KeyPressEventHandler>("OnKeyPress", callback));
}

void Control::OnKeyUpSet(const std::function<void(Control* const c, KeyEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<KeyEventHandler>("OnKeyUp", callback));
}

void Control::OnMouseClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseClick", callback));
}

void Control::OnMouseDownSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseDown", callback));
}

void Control::OnMouseEnterSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnMouseEnter", callback));
}

void Control::OnMouseHoverSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnMouseHover", callback));
}

void Control::OnMouseLeaveSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnMouseLeave", callback));
}

void Control::OnMouseLeftDoubleClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseLeftDoubleClick", callback));
}

void Control::OnMouseMoveSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseMove", callback));
}

void Control::OnMouseRightDoubleClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseRightDoubleClick", callback));
}

void Control::OnMouseUpSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseUp", callback));
}

void Control::OnMouseWheelSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<MouseEventHandler>("OnMouseWheel", callback));
}

void Control::OnVisibleChangedSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept
{
	Events.Register(std::make_unique<EventHandler>("OnVisibledChanged", callback));
}

void Control::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
}

void Control::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
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

Window* Control::GetWindow() const noexcept
{
	if (Parent != nullptr)
	{
		if (Parent->GetType() == typeid(Window))
		{
			return dynamic_cast<Window*>(Parent);
		}

		if (const auto c = dynamic_cast<Menu*>(Parent))
		{
			return c->GetWindow();
		}
	}

	return nullptr;
}