#include "Control.h"
#include "Window.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "MouseEventHandler.h"
#include "KeyPressEventHandler.h"
#include "CancelEventHandler.h"
#include "Window.h"
#include "IntPtr.h"
#include "Application.h"

// Linker for ::_TrackMouseEvent function (NativeWindow.cpp usage)
#pragma comment(lib, "comctl32.lib")
#include <Commctrl.h>

int Control::m_IncrementalTabIndex = 0;
unsigned int Control::m_CurrentIndex = 1;
IntPtr Control::m_OpenedControl = nullptr;

Control::ControlNativeWindow::ControlNativeWindow(Control* control)
	:
	m_Control(control),
	m_Target(this)
{
	
}

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
	Update();
	Dispatch("OnGotFocus", &ArgsDefault);
}

void Control::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus)
{
	m_IsTabSelected = false;
	Update();
	Dispatch("OnLostFocus", &ArgsDefault);
}

void Control::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	switch(vk)
	{
		case VK_TAB:	// Allows the user to change controls by pressing Tab
		{
			Control* newCtl;

			// Previous Control
			if(GetKeyState(VK_SHIFT) & 0x8000)
			{
				newCtl = GetPreviousControl();
			}
			else // Next control
			{
				newCtl = GetNextControl();

			}

			if(newCtl != nullptr)
			{
				SetFocus(static_cast<HWND>(newCtl->GetHandle().ToPointer()));
			}

			break;
		}
	}

	ArgsOnKeyDown = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyDown", &ArgsOnKeyDown);
}

void Control::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	if(m_OpenedControl != nullptr)
	{
		auto wnd = GetWindow();
		auto c = wnd->GetByHandle(m_OpenedControl);
		c->Hide();
		m_OpenedControl = nullptr;
	}

	// Trigger tabbing
	if(Parent != nullptr && GetFocus() != hwnd)
	{
		const auto& newCtl = GetByHandle(hwnd);

		if(newCtl->IsEnabled() && newCtl->IsTabStop())
		{
			SetFocus(static_cast<HWND>(newCtl->GetHandle().ToPointer()));
		}
	}
	else
	{
		Update();
	}

	SetMouseOverState(true);
	SetClickingState(true);
	Update();
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void Control::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext)
{
	if(fNext)
	{
		//HandleMessageForwarder(hwndSetFocus, WM_SETFOCUS, (WPARAM)hwnd, 0);
	}
	else
	{
		if(const auto& ctl = dynamic_cast<Control*>(GetByHandle(hwndSetFocus)))
		{
			auto next = GetByTabIndex(ctl->m_TabIndex + 1);
			//HandleMessageForwarder(hwnd, WM_KILLFOCUS, (WPARAM)static_cast<HWND>(next->GetHandle().ToPointer()), 0);
			//HandleMessageForwarder(static_cast<HWND>(next->GetHandle().ToPointer()), WM_SETFOCUS, (WPARAM)hwnd, 0);
		}
	}
}

Drawing::Rectangle const Control::GetDrawableArea() noexcept
{
	return m_DrawableArea;
}

bool Control::IsHandleCreated() const noexcept
{
	if(m_Window == nullptr) return false;

	return m_Window->Handle != IntPtr::Zero();
}

IntPtr Control::GetHandle() noexcept
{
	if(!IsHandleCreated()) CreateHandle();

	return m_Window->Handle;
}

void Control::Dispose()
{
	Component::Dispose();

	for(auto c : Controls)
	{
		if(c != nullptr)
		{
			c->Dispose();
			delete c; c = nullptr;
		}
	}

	Controls.clear();
	if(m_Graphics != nullptr) m_Graphics->Dispose();
}

void Control::SetHandle(IntPtr value)
{
	if(value == IntPtr::Zero())
	{
		SetState(STATE_CREATED, false);
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

void Control::EnableTabStop() noexcept
{
	if(!IsTabStop())
	{
		m_IsTabStop = true;
	}
}

void Control::DisableTabStop() noexcept
{
	if(IsTabStop())
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
	for(const auto& c : Controls)
	{
		c->SetFont(font);
	}

	m_Font = font;
	m_HasFontChanged = true;
	Update();
}

void Control::SetText(const std::string& text) noexcept
{
	Text = text;
}

Control::Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept
	:
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
	m_DrawableArea(3, 3, width - 3, height - 3),
	m_Size(width, height),
	m_Location(x, y),
	m_Font("Segoe UI", 9, false, false, false, false, GraphicsUnit::Point),
	ArgsOnClosing(CloseReason::None, false),
	ArgsOnClosed(CloseReason::None),
	ArgsOnKeyDown(Keys::None),
	ArgsOnKeyPressed('0'),
	ArgsOnKeyUp(Keys::None),
	ArgsOnMouseMove(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseDown(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseClick(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseUp(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseDoubleClick(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseWheel(MouseButtons::None, 0, 0, 0, 0),
	m_State(0)
{
	m_Window = new ControlNativeWindow(this);

	if(width != 0 && height != 0)
	{
		RECT rect = { 0 };

		CreateParams* cp = CreateParameters();
		AdjustWindowRectEx(&rect, cp->Style, false, cp->ExStyle);
		CreateHandle();
	}
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

void Control::CreateHandle()
{
	Drawing::Rectangle originalBounds;

	CreateParams* cp = CreateParameters();

	if(Parent != nullptr)
	{
		Drawing::Rectangle parentClient = Parent->GetClientRectangle();

		if(!parentClient.IsEmpty())
		{
			if(cp->X != CW_USEDEFAULT)
			{
				cp->X -= parentClient.Left;
			}
			if(cp->Y != CW_USEDEFAULT)
			{
				cp->Y -= parentClient.Left;
			}
		}
	}

	// And if we are WS_CHILD, ensure we have a parent GetHandle().
	if(cp->Parent == IntPtr::Zero() && (cp->Style & WS_CHILD) != 0)
	{
		assert((cp->ExStyle & WS_EX_MDICHILD) == 0, "Can't put MDI child forms on the parking form");
	}

	m_Window->CreateHandle(cp);
}

CreateParams* Control::CreateParameters()
{
	if(m_CreateParams == nullptr) m_CreateParams = new CreateParams();

	CreateParams* cp = m_CreateParams;
	cp->Style = 0;
	cp->ExStyle = 0;
	cp->ClassStyle = 0;
	cp->Caption = Text;

	cp->X = m_Location.X;
	cp->Y = m_Location.Y;
	cp->Width = m_Size.Width;
	cp->Height = m_Size.Height;

	cp->Style = WS_CLIPCHILDREN;
	//if (GetStyle(ControlStyles.ContainerControl)) {
	//	cp.ExStyle |= NativeMethods.WS_EX_CONTROLPARENT;
	//}
	cp->ClassStyle = CS_DBLCLKS;

	if(Parent != nullptr)
	{
		cp->Parent = Parent == nullptr ? IntPtr::Zero() : Parent->GetHandle();
		cp->Style |= WS_CHILD | WS_CLIPSIBLINGS;
	}
	else
	{
		cp->Parent = IntPtr::Zero();
	}

	if(IsTabStop()) cp->Style |= WS_TABSTOP;
	if(IsShown()) cp->Style |= WS_VISIBLE;
	if(IsEnabled()) cp->Style |= WS_DISABLED;

	cp->Param = dynamic_cast<NativeWindow*>(m_Window);

	return cp;
}

void Control::HookMouseEvent()
{
	if(!GetState(STATE_TRACKINGMOUSEEVENT))
	{
		SetState(STATE_TRACKINGMOUSEEVENT, true);

		if(m_TrackMouseEvent == nullptr)
		{
			m_TrackMouseEvent = new TRACKMOUSEEVENT();
			m_TrackMouseEvent->dwFlags = TME_LEAVE | TME_HOVER;
			m_TrackMouseEvent->hwndTrack = GetHandle();
		}

		TrackMouseEvent(m_TrackMouseEvent);
	}
}

void Control::UnhookMouseEvent()
{
	SetState(STATE_TRACKINGMOUSEEVENT, false);
}

Control::~Control() noexcept(false)
{
	if(OnActivate != nullptr) { delete OnActivate; OnActivate = nullptr; }
	if(OnClick != nullptr) { delete OnClick; OnClick = nullptr; }
	if(OnDeactivate != nullptr) { delete OnDeactivate; OnDeactivate = nullptr; }
	if(OnGotFocus != nullptr) { delete OnGotFocus; OnGotFocus = nullptr; }
	if(OnLostFocus != nullptr) { delete OnLostFocus; OnLostFocus = nullptr; }
	if(OnKeyDown != nullptr) { delete OnKeyDown; OnKeyDown = nullptr; }
	if(OnKeyPress != nullptr) { delete OnKeyPress; OnKeyPress = nullptr; }
	if(OnKeyUp != nullptr) { delete OnKeyUp; OnKeyUp = nullptr; }
	if(OnMouseClick != nullptr) { delete OnMouseClick; OnMouseClick = nullptr; }
	if(OnMouseDown != nullptr) { delete OnMouseDown; OnMouseDown = nullptr; }
	if(OnMouseEnter != nullptr) { delete OnMouseEnter; OnMouseEnter = nullptr; }
	if(OnMouseHover != nullptr) { delete OnMouseHover; OnMouseHover = nullptr; }
	if(OnMouseLeave != nullptr) { delete OnMouseLeave; OnMouseLeave = nullptr; }
	if(OnMouseLeftDoubleClick != nullptr) { delete OnMouseLeftDoubleClick; OnMouseLeftDoubleClick = nullptr; }
	if(OnMouseMove != nullptr) { delete OnMouseMove; OnMouseMove = nullptr; }
	if(OnMouseRightDoubleClick != nullptr) { delete OnMouseRightDoubleClick; OnMouseRightDoubleClick = nullptr; }
	if(OnMouseUp != nullptr) { delete OnMouseUp; OnMouseUp = nullptr; }
	if(OnMouseWheel != nullptr) { delete OnMouseWheel; OnMouseWheel = nullptr; }
	if(OnPaint != nullptr) { delete OnPaint; OnPaint = nullptr; }
	if(OnVisibleChanged != nullptr) { delete OnVisibleChanged; OnVisibleChanged = nullptr; }

	SafeDelete(m_Graphics);
	if(DestroyWindow(static_cast<HWND>(GetHandle().ToPointer())) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	SafeDelete(m_CreateParams);
	SafeDelete(m_TrackMouseEvent);
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

void Control::DefWndProc(Message& m)
{
	m_Window->DefWndProc(m);
}

void Control::WndProc(Message& m)
{
	if((m_ControlStyle & ControlStyles::EnableNotifyMessage) == ControlStyles::EnableNotifyMessage)
	{
		OnNotifyMessage(m);
	}

	auto hWnd = m.hWnd;
	auto msg = m.Msg;
	auto wParam = m.wParam;
	auto lParam = m.lParam;

	switch(msg)
	{
		case WM_SETCURSOR:
		{
			/* If an application processes this message, it should return TRUE to halt further processing or FALSE to continue. */
			OnSetCursor_Impl(hWnd, (HWND)wParam, static_cast<unsigned int>(LOWORD(lParam)), static_cast<unsigned int>(HIWORD(lParam)));
			break;
		}
		case WM_CREATE:
		{
			OnCreate_Impl(hWnd, (LPCREATESTRUCT)lParam.ToPointer());
			break;
		}
		case WM_ERASEBKGND:
		{
			OnEraseBackground_Impl(hWnd, (HDC)wParam);
			break;
		}
		case WM_COMMAND:
		{
			OnCommand_Impl(hWnd, static_cast<int>(LOWORD(wParam)), (HWND)lParam, static_cast<unsigned int>(HIWORD(wParam)));
			break;
		}
		case WM_CLOSE:
		{
			OnClosing_Impl(hWnd);
			break;
		}
		case WM_NCDESTROY:
		{
			OnClosed_Impl(hWnd);
			break;
		}
		case WM_SETFOCUS:
		{
			OnFocusEnter_Impl(hWnd, (HWND)wParam);
			break;
		}
		case WM_KILLFOCUS:
		{
			OnFocusLeave_Impl(hWnd, (HWND)wParam);
			break;
		}
		case WM_ACTIVATE:
		{
			OnActivate_Impl(hWnd, static_cast<unsigned int>(LOWORD(wParam)), (HWND)(lParam), static_cast<bool> (HIWORD(wParam)));
			break;
		}
		case WM_SIZE:
		{
			OnSize_Impl(hWnd, (unsigned int)wParam.ToInt32(), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
			break;
		}
		/******************** KEYBOARD MESSAGES *********************/
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:	// Syskey commands need to be handled to track ALT key (VK_MENU)
		{
			OnKeyDown_Impl(hWnd, static_cast<unsigned int>(wParam.ToInt32()), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:	// Syskey commands need to be handled to track ALT key (VK_MENU)
		{
			OnKeyUp_Impl(hWnd, static_cast<unsigned int>(wParam.ToInt32()), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
			break;
		}
		case WM_CHAR:
		{
			OnKeyPressed_Impl(hWnd, static_cast<char>(wParam.ToInt32()), (int)(short)LOWORD(lParam));
			break;
		}
		/******************* END KEYBOARD MESSAGES ******************/

		case WM_GETDLGCODE:
		{
			OnGetDLGCode_Impl(hWnd, (LPMSG)(lParam.ToPointer()));
			break;
		}

		/********************** MOUSE MESSAGES **********************/
		case WM_MOUSEMOVE:
		{
			OnMouseMove_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_LBUTTONDOWN:
		{
			OnMouseLeftDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_MOUSELEAVE:
		{
			OnMouseLeave_Impl(hWnd);
			break;
		}
		case WM_LBUTTONUP:
		{
			OnMouseLeftUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_RBUTTONDOWN:
		{
			OnMouseRightDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_RBUTTONUP:
		{
			OnMouseRightUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			OnMouseLeftDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_RBUTTONDBLCLK:
		{
			OnMouseRightDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam.ToInt32()));
			break;
		}
		case WM_MOUSEWHEEL:
		{
			OnMouseWheel_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam));
			break;
		}
		case WM_SHOWWINDOW:
		{
			OnShowWindow_Impl(hWnd, static_cast<bool>(wParam.ToInt32()), static_cast<unsigned int>(lParam.ToInt32()));
			break;
		}
		case WM_PAINT:
		{
			OnPaint_Impl(hWnd);
			break;
		}
		/******************** END MOUSE MESSAGES ********************/

		/******************** RAW MOUSE MESSAGES ********************/
		case WM_INPUT:
		{
			OnRawInput_Impl(hWnd, GET_RAWINPUT_CODE_WPARAM(wParam.ToInt32()), (HRAWINPUT)lParam.ToPointer());
			break;
		}
		case WM_NEXTDLGCTL:
		{
			OnNextDialogControl_Impl(hWnd, (HWND)wParam, static_cast<bool>(lParam.ToInt32()));
			break;
		}
		/****************** END RAW MOUSE MESSAGES ******************/
		/********************* SCROLL MESSAGES **********************/
		case WM_HSCROLL:
		{
			OnHorizontalScrolling_Impl(hWnd, (HWND)lParam, (unsigned int)LOWORD(wParam), (int)(short)HIWORD(wParam));
			break;
		}
		case WM_VSCROLL:
		{
			OnVerticalScrolling_Impl(hWnd, (HWND)lParam, (unsigned int)LOWORD(wParam), (int)(short)HIWORD(wParam));
			break;
		}
	}
	/******************* END SCROLL MESSAGES ********************/
	DefWndProc(m);
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
	if(m_TabIndex == index)
	{
		return this;
	}

	for(const auto& c : Controls)
	{
		Control* ret = c->GetByTabIndex(index);
		if(ret != nullptr && ret->IsEnabled())
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
	if(const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		auto searchIndex = m_TabIndex == 0 ? m_IncrementalTabIndex - 1 : m_TabIndex - 1;

		for(auto i = searchIndex; i >= 0; --i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if(ret != nullptr && ret->IsEnabled() && ret->IsTabStop())
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
	if(const auto& root = dynamic_cast<Control*>(GetWindow()))
	{
		auto searchIndex = m_TabIndex >= m_IncrementalTabIndex - 1 ? 0 : m_TabIndex + 1;

		for(auto i = searchIndex; i < m_IncrementalTabIndex; ++i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if(ret != nullptr && ret->IsEnabled() && ret->IsTabStop())
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
	if(GetHandle() == p)
	{
		return this;
	}

	for(const auto& c : Controls)
	{
		Control* ret = c->GetByHandle(p);
		if(ret != nullptr)
		{
			return ret;
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

Control* Control::GetById(unsigned int id) noexcept
{
	if(GetId() == id)
	{
		return this;
	}

	for(const auto& c : Controls)
	{
		Control* ret = c->GetById(id);
		if(ret != nullptr)
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
	if(index > m_IncrementalTabIndex)
	{
		m_TabIndex = m_IncrementalTabIndex;
	}
	else
	{
		m_TabIndex = index;
	}
}

Window* Control::GetWindow() noexcept
{
	if(GetType() == typeid(Window))
		return dynamic_cast<Window*>(this);

	if(Parent != nullptr)
	{
		if(Parent->GetType() == typeid(Window))
		{
			return dynamic_cast<Window*>(Parent);
		}

		return Parent->GetWindow();
	}

	return nullptr;
}

Point Control::GetLocation() const noexcept
{
	return m_Location;
}

void Control::SetLocation(Point p) noexcept
{
	m_Location = p;
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void Control::SetLocation(int x, int y) noexcept
{
	m_Location = Point(x, y);
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

Size Control::GetSize() const noexcept
{
	return m_Size;
}

void Control::Resize(Size s)
{
	m_Size = s;
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, s.Width - m_Margin.Right, s.Height - m_Margin.Bottom);
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void Control::Resize(int width, int height)
{
	m_Size = Size(width, height);
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, width - m_Margin.Right, height - m_Margin.Bottom);
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

bool Control::IsMouseOver() const noexcept
{
	return m_IsMouseOver;
}

bool Control::IsClicking() const noexcept
{
	return m_IsClicking;
}

bool Control::IsEnabled() const noexcept
{
	if(!m_Enabled) return false;

	// Check parents if its enabled
	if(Parent != nullptr)
	{
		return Parent->IsEnabled();
	}

	return true;
}

void Control::Enable()
{
	m_Enabled = true;
	EnableWindow(static_cast<HWND>(GetHandle().ToPointer()), true);
}

void Control::Disable()
{
	m_Enabled = false;
	EnableWindow(static_cast<HWND>(GetHandle().ToPointer()), false);
}

void Control::Update()
{
	InvalidateRect(static_cast<HWND>(GetHandle().ToPointer()), nullptr, true);
}

const Graphics* Control::CreateGraphics() const noexcept
{
	return m_Graphics;
}

bool Control::IsShown() const noexcept
{
	return m_IsVisible;
}

void Control::Hide()
{
	if(IsShown())
	{
		m_IsVisible = false;
		ShowWindow(static_cast<HWND>(GetHandle().ToPointer()), SW_HIDE);
	}
}

void Control::Show()
{
	if(!IsShown())
	{
		m_IsVisible = true;
		ShowWindow(static_cast<HWND>(GetHandle().ToPointer()), SW_SHOWDEFAULT);
	}
}

Font Control::GetFont() const noexcept
{
	return m_Font;
}

Color Control::GetBackgroundColor() const noexcept
{
	if((m_BackgroundColor == Color::ControlBackground_Win11() || m_BackgroundColor == Color::ControlBackground_Win10()) && !IsEnabled())
	{
		return Color::DisabledControlBackground();
	}

	return m_BackgroundColor;
}

void Control::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
	Update();
}

Color Control::GetForeColor() const noexcept
{
	if(!IsEnabled()) return Color::DisabledForeground();
	return m_ForeColor;
}

void Control::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
	Update();
}

void Control::ResetMouseEventArgs()
{
	if(GetState(STATE_TRACKINGMOUSEEVENT))
	{
		UnhookMouseEvent();
		HookMouseEvent();
	}
}

unsigned int Control::GetId() const noexcept
{
	return m_Id;
}

void Control::SetMouseOverState(bool state) noexcept
{
	m_IsMouseOver = state;
}

void Control::SetClickingState(bool state) noexcept
{
	m_IsClicking = state;
}

// Default PreDraw function which is used to recalculate elements according to DataSource, number of elements, etc...
void Control::PreDraw(Graphics* const graphics)
{
	auto hwnd = static_cast<HWND>(GetHandle().ToPointer());

	if(m_HasFontChanged)
	{
		graphics->CreateFontObject(m_Font);
		auto hFont = graphics->GetElement(std::string(m_Font.ToString()));
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
		m_HasFontChanged = false;
	}

	graphics->CreateSolidBrush(GetBackgroundColor());
	graphics->CreateSolidBrush(GetForeColor());
}

void Control::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{

}

void Control::PostDraw(Graphics* const graphics)
{

}

void Control::OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized)
{
	/**************************************************************************************************/
	/* The low - order word specifies whether the window is being activated or deactivated. This
	parameter can be one of the following values. The high - order word specifies the minimized state of
	the window being activated or deactivated.A nonzero value indicates the window is minimized.

		LOPARAM = state
		HIPARAM = minimized

		1 = WA_ACTIVE: Activated by some method other than a mouse click (for example, by a call to the
		SetActiveWindow function or by use of the keyboard interface to select the window).

		2 = WA_CLICKACTIVE: Activated by a mouse click.

		0 = WA_INACTIVE: Deactivated.

	A handle to the window being activated or deactivated, depending on the value of the wParam]
	parameter. If the low-order word of wParam is WA_INACTIVE, lParam is the handle to the window being
	activated. If the low-order word of wParam is WA_ACTIVE or WA_CLICKACTIVE, lParam is the handle to
	the window being deactivated. This handle can be NULL.
	/**************************************************************************************************/

	if(state == 1 || state == 2)
	{
		Dispatch("OnActivate", &ArgsDefault);
	}
	else
	{
		Dispatch("OnDeactivate", &ArgsDefault);
	}
}

void Control::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify)
{

}

int Control::OnClosing_Impl(HWND hwnd)
{
	Dispatch("OnClosing", &ArgsOnClosing);

	// If cancel is false and doesn't have Parent Control, means that it's the main window
	// and application must post quit
	if(!ArgsOnClosing.Cancel)
	{
		if(GetType() == typeid(Window))
		{
			Dispose();
			Application::RemoveWindow(dynamic_cast<Window*>(this));
		}

		if(Application::CanCloseApplication())
		{
			Application::Exit();

		}
		return 0;
	}

	return 1;
}

void Control::OnClosed_Impl(HWND hwnd)
{
	Dispatch("OnClosed", &ArgsOnClosed);
}

void Control::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	if(!IsHandleCreated()) CreateHandle();

	m_Graphics = Graphics::Create(hwnd, m_Size);
	m_Graphics->CreateSolidBrush(m_BackgroundColor);
	m_Graphics->CreateSolidBrush(m_ForeColor);
	m_Graphics->CreateFontObject(m_Font);
	Dispatch("OnCreate", &ArgsDefault);
}

void Control::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int Control::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	// 0 always redraw
	// Some controls like have 1 to avoid flickering on input

	return 0;
}


int Control::OnGetDLGCode_Impl(HWND hwnd, LPMSG msg)
{
	return 0;
}

void Control::OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnHorizontalScroll", &ArgsDefault);
}

void Control::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat)
{
	ArgsOnKeyPressed = KeyPressEventArgs(c);
	Dispatch("OnKeyPress", &ArgsOnKeyPressed);
}

void Control::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	ArgsOnKeyUp = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyUp", &ArgsOnKeyUp);
}


void Control::OnMouseLeave_Impl(HWND hwnd)
{
	SetClickingState(false);
	SetMouseOverState(false);
	Update();
	Dispatch("OnMouseLeave", &ArgsDefault);
}

void Control::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	/**************************************************************************************************/
	/* As noted above, the x - coordinate is in the low - order short of the return value; the y -
	coordinate is in the high - order short(both represent signed values because they can take negative
	values on systems with multiple monitors).
	If the return value is assigned to a variable, you can use the MAKEPOINTS macro to obtain a POINTS
	structure from the return value.
	You can also use the GET_X_LPARAM or GET_Y_LPARAM macro to extract the x - or y - coordinate.

	Important

	Do not use the LOWORD or HIWORD macros to extract the x - and y - coordinates of the cursor position
	because these macros return incorrect results on systems with multiple monitors.
	Systems with multiple monitors can have negative x - and y - coordinates, and LOWORDand HIWORD treat
	the coordinates as unsigned quantities.
	/**************************************************************************************************/

	if(!IsMouseOver())
	{
		// Mouseover Redraw should happen on MouseEnter trigger, not on MouseMove
		Update();
		Dispatch("OnMouseEnter", &ArgsDefault);
	}

	TRACKMOUSEEVENT tme = { 0 };
	tme.cbSize = sizeof(TRACKMOUSEEVENT); //Monitor mouse to leave
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = hwnd;
	if(::_TrackMouseEvent(&tme))
	{
		SetMouseOverState(true);
	}

	ArgsOnMouseMove = MouseEventArgs(MouseButtons::None, 0, 0, x, y);
	Dispatch("OnMouseMove", &ArgsOnMouseMove);
}

void Control::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	SetClickingState(false);
	SetMouseOverState(true);
	Update();
	ArgsOnMouseClick = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnClick", &ArgsDefault);
	Dispatch("OnMouseClick", &ArgsOnMouseClick);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void Control::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void Control::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void Control::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Left, 2, 0, x, y);
	Dispatch("OnDoubleClick", &ArgsDefault);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void Control::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Right, 2, 0, x, y);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void Control::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys)
{
	switch(fwKeys)
	{
		case MK_LBUTTON: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::Left, 0, delta, x, y); break;
		case MK_MBUTTON: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::Middle, 0, delta, x, y); break;
		case MK_RBUTTON: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::Right, 0, delta, x, y);  break;
		case MK_XBUTTON1: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::XButton1, 0, delta, x, y); break;
		case MK_XBUTTON2: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::XButton2, 0, delta, x, y); break;
		default: ArgsOnMouseWheel = MouseEventArgs(MouseButtons::None, 0, delta, x, y); break;
	}

	Dispatch("OnMouseWheel", &ArgsOnMouseWheel);
}


int Control::OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys)
{
	return 1;
}

void Control::OnNotifyMessage(Message& m)
{}

void Control::OnPaint_Impl(HWND hwnd)
{
	if(IsShown())
	{
		Drawing::Rectangle rect = Drawing::Rectangle(0, 0, m_Size.Width, m_Size.Height);

		if(m_Graphics == nullptr)
		{
			m_Graphics = Graphics::Create(GetHandle(), m_Size);
		}

		PaintEventArgs pArgs = PaintEventArgs(m_Graphics, rect);

		m_Graphics->BeginDraw();
		PreDraw(m_Graphics);
		Draw(m_Graphics, rect);
		Dispatch("OnPaint", &pArgs);
		m_Graphics->EndDraw();
		PostDraw(m_Graphics);

		//PAINTSTRUCT ps = { 0 };
		//BeginPaint(hwnd, &ps);
		//Direct2D::BeginDraw(this);

		//
		//m_Graphics = Graphics(Handle, ps.hdc);
		//Drawing::Rectangle rect = Drawing::Rectangle(m_Location, m_Size);
		//PaintEventArgs pArgs = PaintEventArgs(m_Graphics, rect);

		//// Perform PreDraw to recalculate elements prior to drawing
		//PreDraw(m_Graphics);

		//// Proceed with the Draw function
		//Draw(m_Graphics, rect);
		//Dispatch("OnPaint", &pArgs);

		//// Call PostDraw to handle additional processing and object cleaning
		//PostDraw(m_Graphics);
		//
		//Direct2D::EndDraw();
		//EndPaint(hwnd, &ps);
	}
}

void Control::OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput)
{

}

int Control::OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg)
{
	return 0;
}

void Control::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy)
{
	m_Size.Width = cx;
	m_Size.Height = cy;

	if(m_Graphics) m_Graphics->UpdateSize(m_Size);

	Dispatch("OnResize", &ArgsDefault);
}

void Control::OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status)
{
	if(fShow)
	{
		Dispatch("OnShown", &ArgsDefault);
	}
	else
	{
		Dispatch("OnHide", &ArgsDefault);
	}

	Dispatch("OnVisibleChanged", &ArgsDefault);
}

void Control::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnVerticalScroll", &ArgsDefault);
}

void Control::ControlNativeWindow::OnHandleChange()
{
	m_Target->OnHandleChange(Handle);
}

void Control::ControlNativeWindow::WndProc(Message& m)
{
	switch(m.Msg)
	{
		case WM_MOUSELEAVE:
		{
			m_Control->UnhookMouseEvent();
			break;
		}
		case WM_MOUSEMOVE:
		{
			if(!m_Control->GetState(Control::STATE_TRACKINGMOUSEEVENT))
			{
				m_Control->HookMouseEvent();
				if(!m_Control->GetState(Control::STATE_MOUSEENTERPENDING))
				{
					m_Control->SendControlMessage(WM_MOUSEENTER, 0, 0);
				}
				else
				{
					m_Control->SetState(Control::STATE_MOUSEENTERPENDING, false);
				}
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			// TrackMouseEvent's mousehover implementation doesn't watch the wheel correctly...
			m_Control->ResetMouseEventArgs();
			break;
		}
	}

	m_Target->OnMessage(m);
}

void Control::ControlNativeWindow::OnHandleChange(IntPtr newHandle)
{
	m_Control->SetHandle(newHandle);
}

void Control::ControlNativeWindow::OnMessage(Message& m)
{
	m_Control->WndProc(m);
}

IntPtr Control::SendControlMessage(int msg, int wparam, int lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, lparam);
}

IntPtr Control::SendControlMessage(int msg, int& wparam, int& lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, lparam);
}
IntPtr Control::SendControlMessage(int msg, int wparam, IntPtr lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, (IntPtr)wparam, lparam);
}

IntPtr Control::SendControlMessage(int msg, IntPtr wparam, IntPtr lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, lparam);
}

IntPtr Control::SendControlMessage(int msg, IntPtr wparam, int lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, (IntPtr)lparam);
}

IntPtr Control::SendControlMessage(int msg, int wparam, LPRECT lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, (LPARAM)lparam);
}

IntPtr Control::SendControlMessage(int msg, bool wparam, int lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, lparam);
}

IntPtr Control::SendControlMessage(int msg, int wparam, const std::string& lparam)
{
	assert(IsHandleCreated(), "Performance alert!  Calling Control::SendMessage and forcing handle creation.  Re-work control so handle creation is not required to set properties.  If there is no work around, wrap the call in an IsHandleCreated check.");
	return SendMessage(static_cast<HWND>(GetHandle()), msg, wparam, (LPARAM)lparam.c_str());
}

int Control::GetWindowStyle()
{
	return (int)(long)GetWindowLong(static_cast<HWND>(GetHandle().ToPointer()), GWL_STYLE);
}

void Control::SetWindowStyle(IntPtr value)
{
	SetWindowLong(static_cast<HWND>(GetHandle().ToPointer()), GWL_STYLE, value.ToInt64());
}