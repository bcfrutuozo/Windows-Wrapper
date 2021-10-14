#include "Control.h"
#include "Window.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "MouseEventHandler.h"
#include "KeyPressEventHandler.h"
#include "CancelEventHandler.h"
#include "Window.h"
#include "IntPtr.h"

int Control::m_IncrementalTabIndex = 0;

void Control::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus)
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
	NativeWindow::OnFocusEnter_Impl(hwnd, hwndOldFocus);
}

void Control::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus)
{
	m_IsTabSelected = false;
	NativeWindow::OnFocusLeave_Impl(hwnd, hwndNewFocus);
}

void Control::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	switch (vk)
	{
	case VK_TAB:	// Allows the user to change controls by pressing Tab
	{
		const Control* newCtl;

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

	NativeWindow::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void Control::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	if (m_OpenedControl != nullptr)
	{
		auto wnd = GetWindow();
		auto c = wnd->GetByHandle(m_OpenedControl);
		c->Hide();
		m_OpenedControl = nullptr;
	}

	// Trigger tabbing
	if (Parent != nullptr && GetFocus() != hwnd)
	{
		const auto& newCtl = GetByHandle(hwnd);

		if (newCtl->IsEnabled() && newCtl->IsTabStop())
		{
			SetFocus(static_cast<HWND>(newCtl->Handle.ToPointer()));
		}
	}
	else
	{
		Update();
	}

	NativeWindow::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void Control::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext)
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

Drawing::Rectangle const Control::GetDrawableArea() noexcept
{
	return m_DrawableArea;
}

void Control::Resize(Size size)
{
	NativeWindow::Resize(size);
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, size.Width - m_Margin.Right, size.Height - m_Margin.Bottom);
}

void Control::Resize(int width, int height)
{
	NativeWindow::Resize(Size(width, height));
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, width - m_Margin.Right, height - m_Margin.Bottom);
}

void Control::Dispose()
{
	NativeWindow::Dispose();

	for (auto c : Controls)
	{
		if (c != nullptr)
		{
			c->Dispose();
			delete c; c = nullptr;
		}
	}

	Controls.clear();
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

void Control::EnableTabStop() noexcept
{
	if (!IsTabStop())
	{
		m_IsTabStop = true;
	}
}

void Control::DisableTabStop() noexcept
{
	if (IsTabStop())
	{
		m_IsTabStop = false;
	}
}

bool Control::IsTabStop() const noexcept
{
	return m_IsTabStop;
}

void Control::SetFont(Font font) noexcept
{
	for (const auto& c : Controls)
	{
		c->SetFont(font);
	}

	NativeWindow::SetFont(font);
}

void Control::SetText(const std::string& text) noexcept
{
	Text = text;
}

Control::Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept
	:
	NativeWindow(width, height, x, y),
	Parent(parent),
	Text(text),
	m_Padding(0),
	m_TabIndex(m_IncrementalTabIndex++),
	m_IsTabSelected(false),
	m_IsTabStop(true),
	m_MinSize(0u),
	OnActivate(nullptr),
	OnClick(nullptr),
	OnDeactivate(nullptr),
	OnGotFocus(nullptr),
	OnLostFocus(nullptr),
	OnKeyDown(nullptr),
	OnKeyPress(nullptr),
	OnKeyUp(nullptr),
	OnMouseClick(nullptr),
	OnMouseDown(nullptr),
	OnMouseEnter(nullptr),
	OnMouseHover(nullptr),
	OnMouseLeave(nullptr),
	OnMouseLeftDoubleClick(nullptr),
	OnMouseMove(nullptr),
	OnMouseRightDoubleClick(nullptr),
	OnMouseUp(nullptr),
	OnMouseWheel(nullptr),
	OnVisibleChanged(nullptr),
	OnPaint(nullptr),
	m_Margin(3, 3, 3, 3),
	m_DrawableArea(3, 3, width - 3, height - 3)
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

Control::~Control() noexcept(false)
{
	if (OnActivate != nullptr) { delete OnActivate; OnActivate = nullptr; }
	if (OnClick != nullptr) { delete OnClick; OnClick = nullptr; }
	if (OnDeactivate != nullptr) { delete OnDeactivate; OnDeactivate = nullptr; }
	if (OnGotFocus != nullptr) { delete OnGotFocus; OnGotFocus = nullptr; }
	if (OnLostFocus != nullptr) { delete OnLostFocus; OnLostFocus = nullptr; }
	if (OnKeyDown != nullptr) { delete OnKeyDown; OnKeyDown = nullptr; }
	if (OnKeyPress != nullptr) { delete OnKeyPress; OnKeyPress = nullptr; }
	if (OnKeyUp != nullptr) { delete OnKeyUp; OnKeyUp = nullptr; }
	if (OnMouseClick != nullptr) { delete OnMouseClick; OnMouseClick = nullptr; }
	if (OnMouseDown != nullptr) { delete OnMouseDown; OnMouseDown = nullptr; }
	if (OnMouseEnter != nullptr) { delete OnMouseEnter; OnMouseEnter = nullptr; }
	if (OnMouseHover != nullptr) { delete OnMouseHover; OnMouseHover = nullptr; }
	if (OnMouseLeave != nullptr) { delete OnMouseLeave; OnMouseLeave = nullptr; }
	if (OnMouseLeftDoubleClick != nullptr) { delete OnMouseLeftDoubleClick; OnMouseLeftDoubleClick = nullptr; }
	if (OnMouseMove != nullptr) { delete OnMouseMove; OnMouseMove = nullptr; }
	if (OnMouseRightDoubleClick != nullptr) { delete OnMouseRightDoubleClick; OnMouseRightDoubleClick = nullptr; }
	if (OnMouseUp != nullptr) { delete OnMouseUp; OnMouseUp = nullptr; }
	if (OnMouseWheel != nullptr) { delete OnMouseWheel; OnMouseWheel = nullptr; }
	if (OnPaint != nullptr) { delete OnPaint; OnPaint = nullptr; }
	if (OnVisibleChanged != nullptr) { delete OnVisibleChanged; OnVisibleChanged = nullptr; }
}

void Control::OnActivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnActivate = new EventHandler("OnActivate", callback);
	Events.Register(OnActivate);
}

void Control::OnClickSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnClick = new EventHandler("OnClick", callback);
	Events.Register(OnClick);
}

void Control::OnDeactivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnDeactivate = new EventHandler("OnDeactivate", callback);
	Events.Register(OnDeactivate);
}

void Control::OnGotFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnGotFocus = new EventHandler("OnGotFocus", callback);
	Events.Register(OnGotFocus);
}

void Control::OnLostFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnLostFocus = new EventHandler("OnLostFocus", callback);
	Events.Register(OnLostFocus);
}

void Control::OnKeyDownSet(const std::function<void(Object*, KeyEventArgs*)>& callback) noexcept
{
	OnKeyDown = new KeyEventHandler("OnKeyDown", callback);
	Events.Register(OnKeyDown);
}

void Control::OnKeyPressSet(const std::function<void(Object*, KeyPressEventArgs*)>& callback) noexcept
{
	OnKeyPress = new KeyPressEventHandler("OnKeyPress", callback);
	Events.Register(OnKeyPress);
}

void Control::OnKeyUpSet(const std::function<void(Object*, KeyEventArgs*)>& callback) noexcept
{
	OnKeyUp = new KeyEventHandler("OnKeyUp", callback);
	Events.Register(OnKeyUp);
}

void Control::OnMouseClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseClick = new MouseEventHandler("OnMouseClick", callback);
	Events.Register(OnMouseClick);
}

void Control::OnMouseDownSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseDown = new MouseEventHandler("OnMouseDown", callback);
	Events.Register(OnMouseDown);
}

void Control::OnMouseEnterSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnMouseEnter = new EventHandler("OnMouseEnter", callback);
	Events.Register(OnMouseEnter);
}

void Control::OnMouseHoverSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnMouseHover = new EventHandler("OnMouseHover", callback);
	Events.Register(OnMouseHover);
}

void Control::OnMouseLeaveSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnMouseLeave = new EventHandler("OnMouseLeave", callback);
	Events.Register(OnMouseLeave);
}

void Control::OnMouseLeftDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseLeftDoubleClick = new MouseEventHandler("OnMouseLeftDoubleClick", callback);
	Events.Register(OnMouseLeftDoubleClick);
}

void Control::OnMouseMoveSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseMove = new MouseEventHandler("OnMouseMove", callback);
	Events.Register(OnMouseMove);
}

void Control::OnMouseRightDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseRightDoubleClick = new MouseEventHandler("OnMouseRightDoubleClick", callback);
	Events.Register(OnMouseRightDoubleClick);
}

void Control::OnMouseUpSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseUp = new MouseEventHandler("OnMouseUp", callback);
	Events.Register(OnMouseUp);
}

void Control::OnMouseWheelSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept
{
	OnMouseWheel = new MouseEventHandler("OnMouseWheel", callback);
	Events.Register(OnMouseWheel);
}

void Control::OnPaintSet(const std::function<void(Object*, PaintEventArgs*)>& callback) noexcept
{
	OnPaint = new PaintEventHandler("OnPaint", callback);
	Events.Register(OnPaint);
}

void Control::OnVisibleChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	OnVisibleChanged = new EventHandler("OnVisibleChanged", callback);
	Events.Register(OnVisibleChanged);
}

Padding Control::GetMargin() const noexcept
{
	return m_Margin;
}

void Control::SetMargin(Padding margin) noexcept
{
	auto size = GetSize();
	m_DrawableArea = Drawing::Rectangle(margin.Left, margin.Top, size.Width - margin.Right, size.Height - margin.Bottom);
	m_Margin = margin;
}

void Control::SetMargin(int left, int top, int right, int bottom) noexcept
{
	SetMargin(Padding(left, top, right, bottom));
}

Control* Control::GetByTabIndex(const int& index) noexcept
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

bool Control::IsTabSelected() const noexcept
{
	return m_IsTabSelected;
}

const std::string& Control::GetText() const noexcept
{
	return Text;
}

Control* Control::GetPreviousControl() noexcept
{
	if (const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		auto searchIndex = m_TabIndex == 0 ? m_IncrementalTabIndex - 1 : m_TabIndex - 1;

		for (auto i = searchIndex; i >= 0; --i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr && ret->IsEnabled() && ret->IsTabStop())
			{
				return ret;
			}
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

bool Control::HasChildren() const noexcept
{
	return Controls.size() > 0;
}

Control* Control::GetNextControl() noexcept
{
	if (const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		auto searchIndex = m_TabIndex >= m_IncrementalTabIndex - 1 ? 0 : m_TabIndex + 1;

		for (auto i = searchIndex; i < m_IncrementalTabIndex; ++i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr && ret->IsEnabled() && ret->IsTabStop())
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

void Control::SetTabIndex(const int& index) noexcept
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

bool Control::IsEnabled() const noexcept
{
	if (!NativeWindow::IsEnabled())
	{
		return false;
	}

	if (Parent != nullptr && !Parent->IsEnabled())
	{
		return false;
	}

	return true;
}

Window* Control::GetWindow() noexcept
{
	if (GetType() == typeid(Window))
		return dynamic_cast<Window*>(this);

	if (Parent != nullptr)
	{
		if (Parent->GetType() == typeid(Window))
		{
			return dynamic_cast<Window*>(Parent);
		}

		return Parent->GetWindow();
	}

	return nullptr;
}