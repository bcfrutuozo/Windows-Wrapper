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
#include "Type.h"
#include "LayoutUtils.h"
#include "BoundsSpecified.h"
#include "IContainerControl.h"
#include "ContainerControl.h"
#include "InvalidateEventArgs.h"

// Linker for ::_TrackMouseEvent function (NativeWindow.cpp usage)
#pragma comment(lib, "comctl32.lib")
#include <Commctrl.h>

bool Control::m_NeedToLoadCOMCtl = true;
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
				//newCtl = GetNextControl();

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

		if(newCtl->IsEnabled() && newCtl->TabStop())
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
	//if(m_Graphics != nullptr) m_Graphics->Dispose();
}

ContainerControl* Control::GetParentContainerControl()
{
	Control* c = Parent;
	while(Parent != nullptr)
	{
		if(const auto cc = (ContainerControl*)Parent)
		{
			return cc;
		}

		c = c->Parent;
	}

	return nullptr;
}

AutoValidate Control::GetAutoValidateForControl(Control* control)
{
	ContainerControl* parent = control->GetParentContainerControl();
	return (parent != nullptr) ? parent->GetAutoValidate() : AutoValidate::EnablePreventFocusChange;
}

bool Control::CacheTextInternal()
{
	return false;
}

void Control::ChildGotFocus(Control* const child)
{
	if(Parent != nullptr) Parent->ChildGotFocus(child);
}

bool Control::GetAnyDisposingInHierarchy() noexcept
{
	Control* up = this;
	bool isDisposing = false;

	while(up != nullptr)
	{
		if(up->IsDisposing())
		{
			isDisposing = true;
			break;
		}

		up = up->Parent;
	}

	return isDisposing;
}

void Control::SetHandle(IntPtr value)
{
	if(value == IntPtr::Zero())
	{
		SetState(STATE_CREATED, false);
	}
}

void Control::PerformLayout(LayoutEventArgs args)
{
	if(GetAnyDisposingInHierarchy()) return;


}

void Control::InitScaling(BoundsSpecified specified)
{
	m_RequiredScaling != (unsigned char)((int)specified & RequiredScalingMask);
}

bool Control::IsFocusManagingContainerControl(Control* const ctl)
{
	return ((ctl->m_ControlStyle & ControlStyles::ContainerControl) == ControlStyles::ContainerControl && (IContainerControl*)ctl);
}

void Control::OnParentInvalidated(InvalidateEventArgs* const e)
{
	if(!RenderTransparent()) return;

	if(IsHandleCreated())
	{
		Drawing::Rectangle clipRect = e->InvalidRect;
		Point offs = GetLocation();
		clipRect.Offset(-offs.X, -offs.Y);
		clipRect = Drawing::Rectangle::Intersect(GetClientRectangle(), clipRect);

		if(clipRect.IsEmpty()) return;

		Invalidate(clipRect);
	}
}

bool Control::ShouldAutoValidate()
{
	return GetAutoValidateForControl(this) != AutoValidate::Disable;
}

void Control::NotifyValidated()
{
	OnValidated(&ArgsDefault);
}

bool Control::NotifyValidating()
{
	CancelEventArgs* ev = new CancelEventArgs();
	OnValidating(ev);
	bool ret = ev->Cancel;
	delete ev;
	return ret;
}

void Control::PaintWithoutErrorHandling(PaintEventArgs* const e, short layer)
{
	//try
	//{
	//
	//}
}

Control::Control() noexcept
	:
	m_Padding(0),
	m_TabIndex(-1),
	m_IsTabSelected(false),
	m_IsTabStop(true),
	m_MinSize(0u),
	OnActivate(nullptr),
	OnClick(nullptr),
	OnDeactivate(nullptr),
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
	m_State(0),
	m_CreateParams(nullptr),
	m_MinimumSize(0, 0),
	m_MaximumSize(0, 0),
	m_Location(0, 0),
	m_AutoScroll(0, 0),
	Parent(nullptr),
	m_IsVisible(false),
	m_BackgroundColor(Color::FromKnownColor(KnownColor::ActiveBorder)),
	m_ForeColor(Color::FromName("SlateBlue"))
{
	m_Window = new ControlNativeWindow(this);
	m_State = STATE_VISIBLE | STATE_ENABLED | STATE_TABSTOP | STATE_CAUSESVALIDATION;
	SetStyle(ControlStyles::AllPaintingInWmPaint |
			 ControlStyles::UserPaint |
			 ControlStyles::StandardClick |
			 ControlStyles::StandardDoubleClick |
			 ControlStyles::UseTextForAccessibility |
			 ControlStyles::Selectable, true);

	if(DefaultMargin() != CommonProperties::DefaultMargin) SetMargin(DefaultMargin());
	if(DefaultMinimumSize() != CommonProperties::DefaultMinimumSize) SetMinimumSize(DefaultMinimumSize());
	if(DefaultMaximumSize() != CommonProperties::DefaultMaximumSize) SetMaximumSize(DefaultMaximumSize());

	Size defaultsSize = DefaultSize();
	m_Width = DefaultSize().Width;
	m_Height = DefaultSize().Height;

	if(m_Width != 0 && m_Height != 0)
	{
		RECT rect = { 0 };

		CreateParams* cp = CreateParameters();
		AdjustWindowRectEx(&rect, cp->Style, false, cp->ExStyle);
		m_ClientWidth = m_Width - (rect.right - rect.left);
		m_ClientHeight = m_Height - (rect.bottom - rect.top);
	}
}

Control::Control(Control* parent, const std::string& text) noexcept
	:
	Control()
{
	Parent = parent;
}

Control::Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept
	:
	Control()
{
	Parent = parent;
	SetLocation(x, y);
	SetSize(Size(width, height));
}

Control::Control(const std::string& text) noexcept
	:
	Control(nullptr, text)
{

}

Control::Control(const std::string& text, int width, int height, int x, int y) noexcept
	:
	Control(nullptr, text, width, height, x, y)
{

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

bool Control::TabStop() const noexcept
{
	return TabStopInternal();
}

void Control::TabStop(bool value)
{
	if(TabStop() != value)
	{
		TabStopInternal(value);
		if(IsHandleCreated()) SetWindowStyle(WS_TABSTOP, value);
		OnTabStopChanged(&ArgsDefault);
	}
}

void Control::SetText(const std::string& text) noexcept
{
	Text = text;
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
				cp->X -= parentClient.X;
			}
			if(cp->Y != CW_USEDEFAULT)
			{
				cp->Y -= parentClient.X;
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
	if(m_NeedToLoadCOMCtl)
	{
		const char* COMctlPath = "C:\\Windows\\System32\\ComCtl32.dll";
		if(GetModuleHandle(COMctlPath) != 0 || LoadLibrary(COMctlPath) != 0) m_NeedToLoadCOMCtl = false;
	}

	if(m_CreateParams == nullptr) m_CreateParams = new CreateParams();

	CreateParams* cp = m_CreateParams;
	cp->Style = 0;
	cp->ExStyle = 0;
	cp->ClassStyle = 0;
	cp->Caption = Text;

	cp->X = m_Location.X;
	cp->Y = m_Location.Y;
	cp->Width = m_Width;
	cp->Height = m_Height;

	cp->Style = WS_CLIPCHILDREN;
	if(GetStyle(ControlStyles::ContainerControl)) cp->ExStyle |= WS_EX_CONTROLPARENT;
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

	if(TabStop()) cp->Style |= WS_TABSTOP;
	if(IsShown()) cp->Style |= WS_VISIBLE;
	if(IsEnabled()) cp->Style |= WS_DISABLED;

	cp->Param = dynamic_cast<NativeWindow*>(m_Window);

	return cp;
}

void Control::OnBoundsUpdate(int x, int y, int width, int height)
{

}

Drawing::Rectangle Control::ApplyBoundsConstraints(int suggestedX, int suggestedY, int proposedWidth, int proposedHeight)
{
	if(GetMaximumSize() != Size::Empty() || GetMinimumSize() != Size::Empty())
	{
		Size maximumSize = LayoutUtils::ConvertZeroToUnbounded(GetMaximumSize());
		Drawing::Rectangle newBounds = Drawing::Rectangle(suggestedX, suggestedY, 0, 0);

		// Clip the size to maximum and inflate it to minimum as necessary.
		newBounds.SetSize(LayoutUtils::IntersectSizes(Size(proposedWidth, proposedHeight), maximumSize));
		newBounds.SetSize(LayoutUtils::UnionSizes(newBounds.GetSize(), GetMinimumSize()));

		return newBounds;
	}

	return Drawing::Rectangle(suggestedX, suggestedY, proposedWidth, proposedHeight);
}

void Control::SetBoundsCore(int x, int y, int width, int height, BoundsSpecified specified)
{
	if(Parent != nullptr)
	{
		// Suspend Layout
	}

	if(m_X != x || m_Y != y || m_Width != width || m_Height != height)
	{
		CommonProperties::UpdateSpecifiedBounds(this, x, y, width, height, specified);

		// Provide control with an opportunity to apply self imposed constraints on its size.
		Drawing::Rectangle adjustedBounds = ApplyBoundsConstraints(x, y, width, height);
		width = adjustedBounds.Width;
		height = adjustedBounds.Height;
		x = adjustedBounds.X;
		y = adjustedBounds.Y;

		if(!IsHandleCreated())
		{
			// Handle is not created, just record our new position and we're done.
			UpdateBounds(x, y, width, height);
		}
		else
		{
			if(!GetState(STATE_SIZELOCKEDBYOS))
			{
				int flags = SWP_NOZORDER | SWP_NOACTIVATE;

				if(m_X == x && m_Y == y)
				{
					flags |= SWP_NOMOVE;
				}
				if(m_Width == width && m_Height == height)
				{
					flags |= SWP_NOSIZE;
				}

				// Give a chance for derived controls to do what they want, just before we resize.
				OnBoundsUpdate(x, y, width, height);

				SetWindowPos(GetHandle(), nullptr, x, y, width, height, flags);
			}
		}
	}

	//finally
	//{

	//	// Initialize the scaling engine.
	//	InitScaling(specified);

	//	if(ParentInternal != null)
	//	{
	//		// Some layout engines (DefaultLayout) base their PreferredSize on
	//		// the bounds of their children.  If we change change the child bounds, we
	//		// need to clear their PreferredSize cache.  The semantics of SetBoundsCore
	//		// is that it does not cause a layout, so we just clear.
	//		CommonProperties.xClearPreferredSizeCache(ParentInternal);

	//		// Cause the current control to initialize its layout (e.g., Anchored controls
	//		// memorize their distance from their parent's edges).  It is your parent's
	//		// LayoutEngine which manages your layout, so we call into the parent's
	//		// LayoutEngine.
	//		ParentInternal.LayoutEngine.InitLayout(this, specified);
	//		ParentInternal.ResumeLayout( /* performLayout = */ true);
	//	#if DEBUG
	//		Debug.Assert(ParentInternal.LayoutSuspendCount == suspendCount, "Suspend/Resume layout mismatch!");
	//	#endif
	//	}
	//}
}

bool Control::IsContainerControl()
{
	return false;
}

IContainerControl* Control::GetContainerControl()
{
	Control* c = this;

	if(c != nullptr && IsContainerControl()) c = c->Parent;

	while(c != nullptr && !IsFocusManagingContainerControl(c)) c = c->Parent;

	return (IContainerControl*)c;
}

bool Control::RenderTransparent()
{
	return GetStyle(ControlStyles::SupportsTransparentBackColor) && GetBackgroundColor().GetA() < 255;
}

const Control* const Control::FromHandleInternal(IntPtr handle)
{
	NativeWindow* w = NativeWindow::FromHandle(handle);
	while(w != nullptr && !(ControlNativeWindow*)w)
	{
		w = w->GetPreviousWindow();
	}

	if(!(ControlNativeWindow*)w)
	{
		return ((ControlNativeWindow*)w)->GetControl();
	}

	return nullptr;
}

Control* Control::FromChildHandleInternal(IntPtr handle)
{
	while(handle != IntPtr::Zero())
	{
		Control* ctl = const_cast<Control*>(FromHandleInternal(handle));
		if(ctl != nullptr) return ctl;

		handle = GetAncestor(handle, GA_PARENT);
	}

	return nullptr;
}

bool Control::IsHostedInWin32DialogManager()
{
	if(!GetState(STATE_CHECKEDHOST))
	{
		Control* topMost = GetTopMostParent();
		if(this != topMost)
		{
			SetState(STATE_HOSTEDINDIALOG, topMost->IsHostedInWin32DialogManager());
		}
		else
		{
			IntPtr parentHandle = ::GetParent(GetHandle());
			IntPtr lastParentHandle = parentHandle;

			std::ostringstream oss;

			SetState(STATE_HOSTEDINDIALOG, false);

			while(parentHandle != IntPtr::Zero())
			{
				int len = GetClassName(lastParentHandle, nullptr, 0);
				len += 5;
				char buffer[256];
				GetClassName(lastParentHandle, buffer, len);
				oss << std::string(buffer);

				if(oss.str() == "#32770")
				{
					SetState(STATE_HOSTEDINDIALOG, true);
					break;
				}

				lastParentHandle = parentHandle;
				parentHandle = ::GetParent(parentHandle);
			}
		}

		SetState(STATE_CHECKEDHOST, true);
	}

	return GetState(STATE_HOSTEDINDIALOG);
}

Control* Control::GetTopMostParent()
{
	Control* control = this;

	while(control->Parent != nullptr)
	{
		control = control->Parent;
	}

	return control;
}

Window* Control::FindWindowInternal()
{
	Control* cur = this;
	while(cur != nullptr && !(Window*)cur)
	{
		cur = cur->Parent;
	}

	return (Window*)cur;
}

bool Control::IsDescendant(Control* descendant)
{
	Control* control = descendant;
	while(control != nullptr)
	{
		if(control == this) return true;
		control = control->Parent;
	}
	return false;
}

void Control::NotifyEnter()
{
	OnEnter(&ArgsDefault);
}

void Control::NotifyLeave()
{
	OnLeave(&ArgsDefault);
}

bool Control::GetValidationCancelled()
{
	if(GetState(STATE_VALIDATIONCANCELLED)) return true;
	else
	{
		Control* parent = Parent;
		if(parent != nullptr) return parent->GetValidationCancelled();
		return false;
	}
}

void Control::SetValidationCancelled(bool value)
{
	SetState(STATE_VALIDATIONCANCELLED, value);
}

bool Control::CanSelectCore()
{
	if((m_ControlStyle & ControlStyles::Selectable) != ControlStyles::Selectable)
	{
		return false;
	}

	for(auto ctl = this; ctl != nullptr; ctl = ctl->Parent)
	{
		if(!ctl->IsEnabled() || !ctl->IsShown()) return false;
	}

	return true;
}

bool Control::TabStopInternal() const noexcept
{
	return (m_State & STATE_TABSTOP) != 0;
}

void Control::TabStopInternal(bool value)
{
	if(TabStopInternal() != value) SetState(STATE_TABSTOP, value);
}

Size Control::SizeFromClientSize(int width, int height)
{
	RECT rect = { 0, 0, width, height };
	CreateParams* cp = CreateParameters();
	AdjustWindowRectEx(&rect, cp->Style, HasMenu(), cp->ExStyle);
	return Size(rect.right - rect.left, rect.bottom - rect.top);
}

void Control::SetClientSizeCore(int x, int y)
{
	SetSize(SizeFromClientSize(x, y));
	m_ClientWidth = x;
	m_ClientHeight = y;
	OnClientSizeChanged(&ArgsDefault);
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

	//SafeDelete(m_Graphics);
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

void Control::OnCausesValidationChanged(EventArgs* const e)
{
	Dispatch("OnCausesValidationChanged", e);
}

void Control::OnClientSizeChanged(EventArgs* const e)
{
	Dispatch("OnClientSizeChanged", e);
}

void Control::OnEnter(EventArgs* const e)
{
	Dispatch("OnEnter", e);
}

void Control::OnGotFocus(EventArgs* const e)
{
	if(Parent != nullptr) Parent->ChildGotFocus(this);
	Dispatch("OnGotFocus", e);
}

void Control::OnMove(EventArgs* const e)
{
	Dispatch("OnMove", e);
	if(RenderTransparent()) Invalidate();
}

void Control::OnLeave(EventArgs* const e)
{
	Dispatch("OnLeave", e);
}

void Control::OnLocationChanged(EventArgs* const e)
{
	OnMove(e);
	Dispatch("OnLocationChanged", e);
}

void Control::OnInvalidated(InvalidateEventArgs* const e)
{
	for(const auto& c : Controls)
	{
		c->OnParentInvalidated(e);
	}

	Dispatch("OnInvalidated", e);
}

void Control::OnPaddingChanged(EventArgs* const e)
{
	if(GetStyle(ControlStyles::ResizeRedraw)) Invalidate();
	Dispatch("OnPaddingChanged", e);
}

void Control::OnResize(EventArgs* const e)
{
	if((m_ControlStyle & ControlStyles::ResizeRedraw) == ControlStyles::ResizeRedraw
	   || GetState(STATE_EXCEPTIONWHILEPAINTING))
	{
		Invalidate();
	}

	Dispatch("OnResize", e);
}

void Control::OnSizeChanged(EventArgs* const e)
{
	OnResize(e);
	Dispatch("OnSizeChanged", e);
}

void Control::OnTabStopChanged(EventArgs* const e)
{
	Dispatch("OnTabStopChanged", e);
}

void Control::OnValidated(EventArgs* const e)
{
	Dispatch("OnValidated", e);
}

void Control::OnValidating(CancelEventArgs* const e)
{
	Dispatch("OnValidating", e);
}

void Control::NotifyInvalidate(Drawing::Rectangle invalidatedArea)
{
	auto event = new InvalidateEventArgs(invalidatedArea);
	OnInvalidated(event);
	delete event;
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
			WmCreate(m);
			break;
		}
		case WM_ERASEBKGND:
		{
			WmEraseBackground(m);
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
	margin = LayoutUtils::ClampNegativePaddingToZero(margin);
	if(margin != m_Margin)
	{
		m_Margin = margin;
		Dispatch("OnMarginChanged", &ArgsDefault);
	}
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
			if(ret != nullptr && ret->IsEnabled() && ret->TabStop())
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

Control* Control::GetNextControl(Control* ctl, bool forward) noexcept
{
	if(!Contains(ctl))
	{
		ctl = this;
	}

	if(forward)
	{
		if(Controls.size() > 0 && (ctl == this || !IsFocusManagingContainerControl(ctl)))
		{
			Control* found = ctl->GetFirstChildControlInTabOrder(/*forward=*/true);
			if(found != nullptr)
			{
				return found;
			}
		}

		while(ctl != this)
		{
			int targetIndex = ctl->m_TabIndex;
			bool hitCtl = false;
			Control* found = nullptr;
			Control* p = ctl->Parent;

			// Cycle through the controls in z-order looking for the one with the next highest
			// tab index.  Because there can be dups, we have to start with the existing tab index and
			// remember to exclude the current control.
			//
			int parentControlCount = 0;

			std::vector<Control*> parentControls = std::vector<Control*>(p->Controls.size());

			for(const auto c : p->Controls)
				parentControls.push_back(c);

			parentControlCount = parentControls.size();

			for(int c = 0; c < parentControlCount; c++)
			{

				// The logic for this is a bit lengthy, so I have broken it into separate
				// caluses:

				// We are not interested in ourself.
				//
				if(parentControls[c] != ctl)
				{

					// We are interested in controls with >= tab indexes to ctl.  We must include those
					// controls with equal indexes to account for duplicate indexes.
					//
					if(parentControls[c]->m_TabIndex >= targetIndex)
					{

						// Check to see if this control replaces the "best match" we've already
						// found.
						//
						if(found == nullptr || found->m_TabIndex > parentControls[c]->m_TabIndex)
						{

							// Finally, check to make sure that if this tab index is the same as ctl,
							// that we've already encountered ctl in the z-order.  If it isn't the same,
							// than we're more than happy with it.
							//
							if(parentControls[c]->m_TabIndex != targetIndex || hitCtl)
							{
								found = parentControls[c];
							}
						}
					}
				}
				else
				{
					// We track when we have encountered "ctl".  We never want to select ctl again, but
					// we want to know when we've seen it in case we find another control with the same tab index.
					//
					hitCtl = true;
				}
			}

			if(found != nullptr)
			{
				return found;
			}

			ctl = ctl->Parent;
		}
	}
	else
	{
		if(ctl != this)
		{

			int targetIndex = ctl->m_TabIndex;
			bool hitCtl = false;
			Control* found = nullptr;
			Control* p = ctl->Parent;
				   
			// Cycle through the controls in reverse z-order looking for the next lowest tab index.  We must
			// start with the same tab index as ctl, because there can be dups.
			//
			int parentControlCount = 0;

			std::vector<Control*> parentControls = std::vector<Control*>(p->Controls.size());

			for(const auto c : p->Controls)
				parentControls.push_back(c);

			parentControlCount = parentControls.size();

			for(int c = parentControlCount - 1; c >= 0; c--)
			{

				// The logic for this is a bit lengthy, so I have broken it into separate
				// caluses:

				// We are not interested in ourself.
				//
				if(parentControls[c] != ctl)
				{

					// We are interested in controls with <= tab indexes to ctl.  We must include those
					// controls with equal indexes to account for duplicate indexes.
					//
					if(parentControls[c]->m_TabIndex <= targetIndex)
					{

						// Check to see if this control replaces the "best match" we've already
						// found.
						//
						if(found == nullptr || found->m_TabIndex < parentControls[c]->m_TabIndex)
						{

							// Finally, check to make sure that if this tab index is the same as ctl,
							// that we've already encountered ctl in the z-order.  If it isn't the same,
							// than we're more than happy with it.
							//
							if(parentControls[c]->m_TabIndex != targetIndex || hitCtl)
							{
								found = parentControls[c];
							}
						}
					}
				}
				else
				{
					// We track when we have encountered "ctl".  We never want to select ctl again, but
					// we want to know when we've seen it in case we find another control with the same tab index.
					//
					hitCtl = true;
				}
			}

			// If we were unable to find a control we should return the control's parent.  However, if that parent is us, return
			// NULL.
			//
			if(found != nullptr)
			{
				ctl = found;
			}
			else
			{
				if(p == this)
				{
					return nullptr;
				}
				else
				{
					return p;
				}
			}
		}

		auto ctlControls = ctl->Controls;

		while(ctlControls.size() > 0 && (ctl == this || !IsFocusManagingContainerControl(ctl)))
		{
			Control* found = ctl->GetFirstChildControlInTabOrder(/*forward=*/false);
			if(found != nullptr)
			{
				ctl = found;
				ctlControls = ctl->Controls;
			}
			else
			{
				break;
			}
		}
	}

	return ctl == this ? nullptr : ctl;
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
	//if(GetType() == typeid(Window))
	//	return dynamic_cast<Window*>(this);
	//
	//if(Parent != nullptr)
	//{
	//	if(Parent->GetType() == typeid(Window))
	//	{
	//		return dynamic_cast<Window*>(Parent);
	//	}
	//
	//	return Parent->GetWindow();
	//}
	//
	return nullptr;
}

Point Control::GetLocation() const noexcept
{
	return m_Location;
}

void Control::SetLocation(Point p) noexcept
{
	m_Location = p;
	//SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, GetSize().Width, GetSize().Height, SWP_NOZORDER);
}

void Control::SetLocation(int x, int y) noexcept
{
	m_Location = Point(x, y);
	m_X = x;
	m_Y = y;
	//SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, GetSize().Width, GetSize().Height, SWP_NOZORDER);
}

Size Control::GetSize() const noexcept
{
	return Size(m_Width, m_Height);
}

void Control::SetSize(Size value)
{
	SetBounds(m_X, m_Y, value.Width, value.Height, BoundsSpecified::None);
}

void Control::Resize(Size s)
{
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, s.Width - m_Margin.Right, s.Height - m_Margin.Bottom);
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, GetSize().Width, GetSize().Height, SWP_NOZORDER);
}

void Control::Resize(int width, int height)
{
	m_DrawableArea = Drawing::Rectangle(m_Margin.Left, m_Margin.Top, width - m_Margin.Right, height - m_Margin.Bottom);
	SetWindowPos(static_cast<HWND>(GetHandle().ToPointer()), nullptr, m_Location.X, m_Location.Y, GetSize().Width, GetSize().Height, SWP_NOZORDER);
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

//const Graphics* Control::CreateGraphics() const noexcept
//{
//	return m_Graphics;
//}

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

bool Control::ContainsFocus()
{
	if(!IsHandleCreated()) return false;

	IntPtr focusHwnd = GetFocus();

	if(focusHwnd == IntPtr::Zero()) return false;
	if(focusHwnd == GetHandle()) return true;
	if(IsChild(GetHandle(), focusHwnd)) return true;

	return false;
}

Color Control::GetBackgroundColor() const noexcept
{
	//if((m_BackgroundColor == Color::ControlBackground_Win11() || m_BackgroundColor == Color::ControlBackground_Win10()) && !IsEnabled())
	//{
	//	return Color::DisabledControlBackground();
	//}

	return m_BackgroundColor;
}

void Control::SetBackgroundColor(const Color& color) noexcept
{
	//m_BackgroundColor = color;
	Update();
}

Color Control::GetForeColor() const noexcept
{
	//if(!IsEnabled()) return Color::DisabledForeground();
	return m_ForeColor;
}

void Control::SetForeColor(const Color& color) noexcept
{
	//m_ForeColor = color;
	Update();
}

void Control::PerformLayout()
{
	PerformLayout(nullptr, "");
}

bool Control::IsMirrored()
{
	if(!IsHandleCreated())
	{
		CreateParams* cp = CreateParameters();
		SetState(STATE_MIRRORED, (cp->ExStyle & WS_EX_LAYOUTRTL) != 0);
	}

	return GetState(STATE_MIRRORED);
}

RightToLeft Control::IsRightToLeft()
{
	if(m_RightToLeft == RightToLeft::Inherit)
	{
		Control* parent = Parent;
		if(parent != nullptr) return parent->IsRightToLeft();
		else return DefaultRightToLeft();
	}
}

Padding Control::GetPadding() const noexcept
{
	return m_Padding;
}

void Control::SetPadding(Padding value)
{
	if(value != m_Padding)
	{

	}
}

Size Control::GetMaximumSize() const noexcept
{
	return m_MaximumSize;
}

Size Control::GetMinimumSize() const noexcept
{
	return m_MinimumSize;
}

void Control::SetMaximumSize(Size value)
{
	m_MaximumSize = value;
}

void Control::SetMinimumSize(Size value)
{
	m_MinimumSize = value;
}

Drawing::Rectangle Control::GetRectangle() const noexcept
{
	return Drawing::Rectangle(m_X, m_Y, m_Width, m_Height);
}

void Control::SetRectangle(Drawing::Rectangle rectangle)
{
	SetBounds(rectangle.X, rectangle.Y, rectangle.Width, rectangle.Height, BoundsSpecified::All);
}

void Control::SetBounds(int x, int y, int width, int height, BoundsSpecified specified)
{
	if((specified & BoundsSpecified::X) == BoundsSpecified::None) x = m_X;
	if((specified & BoundsSpecified::Y) == BoundsSpecified::None) y = m_Y;
	if((specified & BoundsSpecified::Width) == BoundsSpecified::None) width = m_Width;
	if((specified & BoundsSpecified::Height) == BoundsSpecified::None) height = m_Height;
	if(m_X != x || m_Y != y || m_Width != width || m_Height != height)
	{
		SetBoundsCore(x, y, width, height, specified);

		// WM_WINDOWPOSCHANGED will trickle down to an OnResize() which will
		// have refreshed the interior layout or the resized control.  We only need to layout
		// the parent.  This happens after InitLayout has been invoked.
		//LayoutTransaction.DoLayout(ParentInternal, this, PropertyNames.Bounds);
	}
	else
	{
		// Still need to init scaling.
		InitScaling(specified);
	}
}

void Control::SetBounds(Drawing::Rectangle bounds, BoundsSpecified specified)
{
	ISite* site = GetSite();
	//IComponentChangeService ccs = null;
	//PropertyDescriptor sizeProperty = null;
	//PropertyDescriptor locationProperty = null;
	bool sizeChanged = false;
	bool locationChanged = false;

	//if(site != nullptr && site->IsDesignMode())
	//{
	//	//ccs = (IComponentChangeService)site.GetService(typeof(IComponentChangeService));
	//	if(ccs != null)
	//	{
	//		sizeProperty = TypeDescriptor.GetProperties(this)[PropertyNames.Size];
	//		locationProperty = TypeDescriptor.GetProperties(this)[PropertyNames.Location];
	//		Debug.Assert(sizeProperty != null && locationProperty != null, "Error retrieving Size/Location properties on Control.");
	//
	//		try
	//		{
	//			if(sizeProperty != null && !sizeProperty.IsReadOnly && (bounds.Width != this.Width || bounds.Height != this.Height))
	//			{
	//				if(!(site is INestedSite))
	//				{
	//					ccs.OnComponentChanging(this, sizeProperty);
	//				}
	//				sizeChanged = true;
	//			}
	//			if(locationProperty != null && !locationProperty.IsReadOnly && (bounds.X != this.x || bounds.Y != this.y))
	//			{
	//				if(!(site is INestedSite))
	//				{
	//					ccs.OnComponentChanging(this, locationProperty);
	//				}
	//				locationChanged = true;
	//			}
	//		}
	//		catch(InvalidOperationException)
	//		{
	//			// The component change events can throw InvalidOperationException if a change is
	//			// currently not allowed (typically because the doc data in VS is locked). 
	//			// When this happens, we just eat the exception and proceed with the change.
	//		}
	//	}
	//}

	SetBoundsCore(bounds.X, bounds.Y, bounds.Width, bounds.Height, specified);

	//if(site != nullptr && ccs != null)
	//{
	//	try
	//	{
	//		if(sizeChanged)  ccs.OnComponentChanged(this, sizeProperty, null, null);
	//		if(locationChanged) ccs.OnComponentChanged(this, locationProperty, null, null);
	//	}
	//	catch(InvalidOperationException)
	//	{
	//		// The component change events can throw InvalidOperationException if a change is
	//		// currently not allowed (typically because the doc data in VS is locked). 
	//		// When this happens, we just eat the exception and proceed with the change.
	//	}
	//}
}

Size Control::GetPreferredSize(Size proposedSize) noexcept
{
	if(GetState(STATE_DISPOSING | STATE_DISPOSED)) return Size(0, 0);
	else return LayoutUtils::ConvertZeroToUnbounded(proposedSize);

}

Drawing::Rectangle Control::GetDisplayRectangle() const noexcept
{
	return Drawing::Rectangle(0, 0, m_ClientWidth, m_ClientHeight);
}

bool Control::GetParticipatesInLayout() noexcept
{
	return GetState(STATE_VISIBLE);
}

void Control::PerformLayout(IArrangedElement* affectedElement, const std::string& property)
{
	PerformLayout(LayoutEventArgs(affectedElement, property));
}

IArrangedElement* Control::GetContainer() const noexcept
{
	return Parent;
}

std::vector<IArrangedElement*> Control::GetChildren() const noexcept
{
	return std::vector<IArrangedElement*>();
}

bool Control::IsDisposing() noexcept
{
	return GetState(STATE_DISPOSING);
}

bool Control::Contains(Control* control)
{
	while(control != nullptr)
	{
		control = control->Parent;
		if(control == nullptr) return false;
		if(control == this) return true;
	}

	return false;
}

bool Control::IsFocused() noexcept
{
	return IsHandleCreated() && GetFocus() == GetHandle().ToPointer();
}

Control* Control::GetParent() const noexcept
{
	return Parent;
}

void Control::Invalidate()
{
	Invalidate(false);
}

void Control::Invalidate(bool invalidateChildren)
{
	if(IsHandleCreated())
	{
		if(invalidateChildren)
		{
			RedrawWindow(GetHandle(), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		}
		else
		{
			InvalidateRect(GetHandle(), nullptr, (m_ControlStyle & ControlStyles::Opaque) != ControlStyles::Opaque);
		}

		NotifyInvalidate(GetClientRectangle());
	}
}

void Control::Invalidate(Drawing::Rectangle rc)
{
	Invalidate(rc, false);
}

void Control::Invalidate(Drawing::Rectangle rc, bool invalidateChildren)
{
	if(rc.IsEmpty())
	{
		Invalidate(invalidateChildren);
	}
	else if(IsHandleCreated())
	{
		RECT rcArea = { rc.X, rc.Y, rc.Width, rc.Height };

		if(invalidateChildren)
		{
			RedrawWindow(GetHandle(), &rcArea, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		}
		else
		{
			InvalidateRect(GetHandle(), &rcArea, (m_ControlStyle & ControlStyles::Opaque) != ControlStyles::Opaque);
		}
	}

	NotifyInvalidate(rc);
}

Drawing::Rectangle Control::RectangleToClient(Drawing::Rectangle r)
{
	RECT rect = { r.X, r.Y, r.X + r.Width, r.Y + r.Height };
	MapWindowPoints(nullptr, GetHandle(), (LPPOINT)(&rect), 2);
	return Drawing::Rectangle::FromLTRB(rect.left, rect.top, rect.right, rect.bottom);
}

Drawing::Rectangle Control::RectangleToScreen(Drawing::Rectangle r)
{
	RECT rect = { r.X, r.Y, r.X + r.Width, r.Y + r.Height };
	MapWindowPoints(GetHandle(), nullptr, (LPPOINT)(&rect), 2);
	return Drawing::Rectangle::FromLTRB(rect.left, rect.top, rect.right, rect.bottom);
}

Point Control::GetAutoScrollOffset() const noexcept
{
	return m_AutoScroll;
}

void Control::SetAutoScrollOffse(Point p)
{
	m_AutoScroll = p;
}

void Control::SetAutoScrollOffse(int x, int y)
{
	m_AutoScroll = Point(x, y);
}

bool Control::CausesValidation()
{
	return GetState(STATE_CAUSESVALIDATION);
}

void Control::CausesValidation(bool value)
{
	if(value != CausesValidation())
	{
		SetState(STATE_CAUSESVALIDATION, value);
		OnCausesValidationChanged(&ArgsDefault);
	}
}

bool Control::CanSelect()
{
	return CanSelectCore();
}

Size Control::GetClientSize()
{
	return Size(m_ClientWidth, m_ClientHeight);
}

void Control::SetClientSize(Size value)
{
	SetClientSizeCore(value.Width, value.Height);
}

void Control::ResetMouseEventArgs()
{
	if(GetState(STATE_TRACKINGMOUSEEVENT))
	{
		UnhookMouseEvent();
		HookMouseEvent();
	}
}

bool Control::HasMenu()
{
	return false;
}

bool Control::GetTopLevel() const noexcept
{
	return (m_State & STATE_TOPLEVEL) != 0;
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

void Control::UpdateBounds()
{
	RECT rect;
	GetClientRect(GetHandle(), &rect);
	int clientWidth = rect.right;
	int clientHeight = rect.bottom;

	GetWindowRect(GetHandle(), &rect);

	if(!GetTopLevel()) MapWindowPoints(nullptr, GetParent()->GetHandle(), (LPPOINT)(&rect), 2);

	UpdateBounds(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, clientWidth, clientHeight);
}

void Control::UpdateBounds(int x, int y, int width, int height)
{
	assert(!IsHandleCreated(), "Don't call this function when handle is created!");

	RECT rect = { 0 };
	CreateParams* cp = CreateParameters();

	AdjustWindowRectEx(&rect, cp->Style, false, cp->ExStyle);
	int clientWidth = width - (rect.right - rect.left);
	int clientHeight = height - (rect.bottom - rect.top);
	UpdateBounds(x, y, width, height, clientWidth, clientHeight);
}

void Control::UpdateBounds(int x, int y, int width, int height, int clientWidth, int clientHeight)
{
	bool newLocation = m_X != x || m_Y != y;
	bool newSize = m_Width != width || m_Height != height || m_ClientWidth != clientWidth || m_ClientHeight != clientHeight;

	m_X = x;
	m_Y = y;
	m_Width = width;
	m_Height = height;
	m_ClientWidth = clientWidth;
	m_ClientHeight = clientHeight;

	if(newLocation) OnLocationChanged(&ArgsDefault);
	if(newSize)
	{
		OnSizeChanged(&ArgsDefault);
		OnClientSizeChanged(&ArgsDefault);
	}
}

void Control::InvokeGotFocus(Control* const toInvoke, EventArgs* const e)
{
	if(toInvoke != nullptr)
	{
		toInvoke->OnGotFocus(e);
	}
}

bool Control::PerformControlValidation(bool bulkValidation)
{
	if(!CausesValidation()) return false;

	if(NotifyValidating()) return true;

	if(bulkValidation)
	{
		NotifyValidated();
	}
	else
	{
		try
		{
			NotifyValidated();
		}
		catch(const Exception& e)
		{

		}
	}
}

Control* Control::GetFirstChildControlInTabOrder(bool forward)
{
	Control* found = nullptr;

	if(forward)
	{
		for(auto it = Controls.begin(); it != Controls.end(); ++it)
		{
			if(found == nullptr || found->m_TabIndex > (*it)->m_TabIndex)
			{
				found = *it;
			}
		}
	}
	else
	{
		for(auto it = Controls.rbegin(); it != Controls.rend(); ++it)
		{
			if(found == nullptr || found->m_TabIndex < (*it)->m_TabIndex)
			{
				found = *it;
			}
		}
	}

	return found;
}

void Control::NotifyValidationResult(Object* const sender, CancelEventArgs* const e)
{
	SetValidationCancelled(e->Cancel);
}

// Default PreDraw function which is used to recalculate elements according to DataSource, number of elements, etc...
//void Control::PreDraw(Graphics* const graphics)
//{
//	auto hwnd = static_cast<HWND>(GetHandle().ToPointer());
//
//	if(m_HasFontChanged)
//	{
//		graphics->CreateFontObject(m_Font);
//		auto hFont = graphics->GetElement(std::string(m_Font.ToString()));
//		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
//		m_HasFontChanged = false;
//	}
//
//	graphics->CreateSolidBrush(GetBackgroundColor());
//	graphics->CreateSolidBrush(GetForeColor());
//}

//void Control::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
//{
//
//}

//void Control::PostDraw(Graphics* const graphics)
//{
//
//}

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
		//if(GetType() == typeid(Window))
		//{
		//	Dispose();
		//	Application::RemoveWindow(dynamic_cast<Window*>(this));
		//}

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

void Control::WmCreate(Message& m)
{
	DefWndProc(m);

	//if(Parent != nullptr) Parent->UpdateChildZOrder(this);

	UpdateBounds();
	//OnHandleCreated(&ArgsDefault);

	if(!GetStyle(ControlStyles::CacheText)) Text = "";

	Dispatch("OnCreate", &ArgsDefault);
}

void Control::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

void Control::WmEraseBackground(Message& m)
{
	if(GetStyle(ControlStyles::UserPaint))
	{
		if(!GetStyle(ControlStyles::AllPaintingInWmPaint))
		{
			IntPtr dc = m.wParam;
			if(dc == IntPtr::Zero())
			{
				m.Result = (void*)0;
				return;
			}

			RECT rc = { 0 };
			GetClientRect(GetHandle(), &rc);
			//PaintEventArgs* pevent = new PaintEventArgs(dc, Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom));
			//PaintWithoutErrorHandling(pevent, PaintLayerBackground);
		}

		m.Result = (void*)1;
	}
	else
	{
		DefWndProc(m);
	}
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
		Drawing::Rectangle rect = Drawing::Rectangle(0, 0, GetSize().Width, GetSize().Height);

		//if(m_Graphics == nullptr)
		//{
		//	m_Graphics = Graphics::Create(GetHandle(), GetSize());
		//}

		//PaintEventArgs pArgs = PaintEventArgs(m_Graphics, rect);
		//
		//m_Graphics->BeginDraw();
		//PreDraw(m_Graphics);
		//Draw(m_Graphics, rect);
		//Dispatch("OnPaint", &pArgs);
		//m_Graphics->EndDraw();
		//PostDraw(m_Graphics);

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

void Control::SetWindowStyle(int flag, bool value)
{
	int styleFlags = ((int)((long)GetWindowLong(GetHandle(), GWL_STYLE)));
	SetWindowLong(GetHandle(), GWL_STYLE, (long)(value ? styleFlags | flag : styleFlags & ~flag));
}
