#include "Control.h"
#include "Window.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "MouseEventHandler.h"
#include "KeyPressEventHandler.h"
#include "CancelEventHandler.h"

unsigned int Control::m_IncrementalTabIndex = 0;

void Control::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
{
	/**************************************************************************************************/
	/* Remarks
		When you change the focus by using the keyboard(TAB, SHIFT + TAB, and so on), by calling the
		Select or SelectNextControl methods, or by setting the ContainerControl.ActiveControl property
		to the current form, focus events occur in the following order:

			Enter
			GotFocus
			Leave
			Validating
			Validated
			LostFocus

		When you change the focus by using the mouse or by calling the Focus method, focus events occur
		in the following order:

			Enter
			GotFocus
			LostFocus
			Leave
			Validating
			Validated

		If the CausesValidation property is set to false, the Validating and Validated events are
		suppressed.
	/**************************************************************************************************/
	m_IsTabSelected = true;
	WinControl::OnFocusEnter_Impl(hwnd, hwndOldFocus);
}

void Control::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	m_IsTabSelected = false;
	WinControl::OnFocusLeave_Impl(hwnd, hwndNewFocus);
}

void Control::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_TAB:	// Allows the user to change controls by pressing Tab
	{
		const WinControl* newCtl;

		// Previous Control
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			newCtl = GetPreviousControl();
		}
		else // Next control
		{
			newCtl = GetNextControl();

		}

		if (newCtl != nullptr)
		{
			SetFocus(static_cast<HWND>(newCtl->Handle.ToPointer()));
		}

		break;
	}
	}

	WinControl::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void Control::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	// Trigger tabbing
	if (Parent != nullptr && GetFocus() != hwnd)
	{
		const WinControl* newCtl = dynamic_cast<WinControl*>(GetByHandle(hwnd));

		if (newCtl != nullptr && newCtl->IsEnabled())
		{
			SetFocus(static_cast<HWND>(newCtl->Handle.ToPointer()));
		}
	}
	else
	{
		Update();
	}

	WinControl::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void Control::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept
{
	if (fNext)
	{
		HandleMessageForwarder(hwndSetFocus, WM_SETFOCUS, (WPARAM)hwnd, 0);
	}
	else
	{
		if (const auto& ctl = dynamic_cast<Control*>(GetByHandle(hwndSetFocus)))
		{
			auto next = GetByTabIndex(ctl->m_TabIndex + 1);
			HandleMessageForwarder(hwnd, WM_KILLFOCUS, (WPARAM)static_cast<HWND>(next->Handle.ToPointer()), 0);
			HandleMessageForwarder(static_cast<HWND>(next->Handle.ToPointer()), WM_SETFOCUS, (WPARAM)hwnd, 0);
		}
	}
}

Control::Control() noexcept
	:
	Control(nullptr, "")
{

}

Control::Control(Control* parent, const std::string& text) noexcept
	:
	Control(parent, text, 0, 0, 0, 0)
{

}

Control::Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept
	:
	WinControl(parent, text, width, height),
	Location(x, y),
	m_BackgroundColor(Color::Control()),
	m_ForeColor(Color::Black()),
	m_Padding(0),
	m_Margin(0),
	m_TabIndex(m_IncrementalTabIndex++),
	m_IsTabSelected(false)
{

}

Control::Control(const std::string& text) noexcept
	:
	Control(nullptr, text, 0, 0, 0, 0)
{

}

Control::Control(const std::string& text, int width, int height, int x, int y) noexcept
	:
	Control(nullptr, text, width, height, x, y)
{

}

Control::~Control() noexcept
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

Size Control::GetSize() const noexcept
{
	return m_Size;
}

Padding Control::GetMargin() const noexcept
{
	return m_Margin;
}

Control* Control::GetByTabIndex(const unsigned int& index) noexcept
{
	if (m_TabIndex == index)
	{
		return this;
	}

	for (const auto& c : Controls)
	{
		Control* ret = c->GetByTabIndex(index);
		if (ret != nullptr && ret->IsEnabled())
		{
			return ret;
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

void Control::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
	Update();
}

void Control::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
	Update();
}

const std::string& Control::GetText() const noexcept
{
	return Text;
}

Control* Control::GetPreviousControl() noexcept
{
	if (const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		int searchIndex = m_TabIndex == 0 ? m_IncrementalTabIndex - 1 : m_TabIndex - 1;

		for (int i = searchIndex; i >= 0; --i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr && ret->IsEnabled())
			{
				return ret;
			}
		}
	}
}

Control* Control::GetNextControl() noexcept
{
	if (const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		int searchIndex = m_TabIndex >= m_IncrementalTabIndex - 1 ? 0 : m_TabIndex + 1;

		for (int i = searchIndex; i < m_IncrementalTabIndex; ++i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr && ret->IsEnabled())
			{
				return ret;
			}
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

Control* Control::GetById(unsigned int id) noexcept
{
	if (GetId() == id)
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

int Control::GetTabIndex() const noexcept
{
	return m_TabIndex;
}

void Control::SetTabIndex(const unsigned int& index) noexcept
{
	// Don't let the user use a higher Tab Index than the number of current controls
	if (index > m_IncrementalTabIndex)
	{
		m_TabIndex = m_IncrementalTabIndex;
	}
	else
	{
		m_TabIndex = index;
	}
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