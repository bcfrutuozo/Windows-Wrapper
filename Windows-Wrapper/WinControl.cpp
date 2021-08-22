#pragma once

#include "WinControl.h"
#include "Window.h"

unsigned int WinControl::m_IncrementalTabIndex = 0;

void WinControl::OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized) noexcept
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

	if (state == 1 || state == 2)
	{
		Dispatch("OnActivate", &ArgsDefault);
	}
	else
	{
		Dispatch("OnDeactivate", &ArgsDefault);
	}
}

void WinControl::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept
{

}

int WinControl::OnClosing_Impl(HWND hwnd) noexcept
{
	Dispatch("OnClosing", &ArgsOnClosing);

	// If cancel is false and doesn't have Parent Control, means that it's the main window
	// and application must post quit
	if (!ArgsOnClosing.Cancel && Parent == nullptr)
	{
		PostQuitMessage(0);
	}

	return 0;
}

void WinControl::OnClosed_Impl(HWND hwnd) noexcept
{
	Dispatch("OnClosed", &ArgsOnClosed);
}

HBRUSH WinControl::OnColorControl_Impl(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
	return CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
}

void WinControl::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) noexcept
{
	Dispatch("OnCreate", &ArgsDefault);
}

void WinControl::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int WinControl::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	// 0 always redraw
	// Some controls like have 1 to avoid flickering on input

	return 0;
}

void WinControl::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
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

void WinControl::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	m_IsTabSelected = false;
	Update();
	Dispatch("OnLostFocus", &ArgsDefault);
}

int WinControl::OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept
{
	return 0;
}

void WinControl::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
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

	ArgsOnKeyDown = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyDown", &ArgsOnKeyDown);
}

void WinControl::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	ArgsOnKeyPressed = KeyPressEventArgs(c);
	Dispatch("OnKeyPress", &ArgsOnKeyPressed);
}

void WinControl::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	ArgsOnKeyUp = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyUp", &ArgsOnKeyUp);
}


void WinControl::OnMouseLeave_Impl(HWND hwnd) noexcept
{
	SetClickingState(false);
	SetMouseOverState(false);
	Update();
	Dispatch("OnMouseLeave", &ArgsDefault);
}

void WinControl::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
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

	if (!IsMouseOver())
	{
		// Mouseover Redraw should happen on MouseEnter trigger, not on MouseMove
		Update();
		Dispatch("OnMouseEnter", &ArgsDefault);
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT); //Monitor mouse to leave
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = hwnd;
	if (::_TrackMouseEvent(&tme))
	{
		SetMouseOverState(true);
	}

	ArgsOnMouseMove = MouseEventArgs(MouseButtons::None, 0, 0, x, y);
	Dispatch("OnMouseMove", &ArgsOnMouseMove);
}

void WinControl::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
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

	SetMouseOverState(true);
	SetClickingState(true);
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinControl::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
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

void WinControl::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinControl::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void WinControl::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Left, 2, 0, x, y);
	Dispatch("OnDoubleClick", &ArgsDefault);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinControl::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Right, 2, 0, x, y);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinControl::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys) noexcept
{
	switch (fwKeys)
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

void WinControl::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept
{
	if (fNext)
	{
		HandleMessageForwarder(hwndSetFocus, WM_SETFOCUS, (WPARAM)hwnd, 0);
	}
	else
	{
		if (const auto& ctl = dynamic_cast<WinControl*>(GetByHandle(hwndSetFocus)))
		{
			auto next = GetByTabIndex(ctl->m_TabIndex + 1);
			HandleMessageForwarder(hwnd, WM_KILLFOCUS, (WPARAM)static_cast<HWND>(next->Handle.ToPointer()), 0);
			HandleMessageForwarder(static_cast<HWND>(next->Handle.ToPointer()), WM_SETFOCUS, (WPARAM)hwnd, 0);
		}
	}
}

int WinControl::OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys) noexcept
{
	return 1;
}

void WinControl::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	// TODO: OnPaint MUST receive a Graphics object which abstracts the PAINTSTRUCT and DeviceContext handle 
	// to let the user customize the control.
	//Dispatch("OnPaint", new PaintEventArgs());

	HFONT hFont = CreateFont(Font.GetSizeInPixels(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());

	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB())));
	EndPaint(hwnd, &ps);
}

void WinControl::OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput) noexcept
{

}

int WinControl::OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg) noexcept
{
	return 0;
}

void WinControl::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept
{

}

void WinControl::OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status) noexcept
{
	if (fShow)
	{
		Dispatch("OnShown", &ArgsDefault);
	}
	else
	{
		Dispatch("OnHide", &ArgsDefault);
	}

	Dispatch("OnVisibleChanged", &ArgsDefault);
}

WinControl* WinControl::GetByTabIndex(const unsigned int& index) noexcept
{
	if (m_TabIndex == index)
	{
		return this;
	}

	for (const auto& c : Controls)
	{
		if (const auto& winc = dynamic_cast<WinControl*>(c.get()))
		{
			WinControl* ret = winc->GetByTabIndex(index);
			if (ret != nullptr && ret->IsEnabled())
			{
				return ret;
			}
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
}

WinControl::WinControl(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept
	:
	Control(parent, text, width, height, x, y),
	m_TabIndex(m_IncrementalTabIndex++),
	m_IsTabSelected(false),
	Font("Segoe", 9.0f, false, false, false, false, GraphicsUnit::Point),		// Default application font for controls
	ArgsOnClosing(CloseReason::None, false),
	ArgsOnClosed(CloseReason::None),
	ArgsOnKeyDown(Keys::None),
	ArgsOnKeyPressed('\0'),
	ArgsOnKeyUp(Keys::None),
	ArgsOnMouseMove(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseDown(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseClick(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseUp(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseDoubleClick(MouseButtons::None, 0, 0, 0, 0),
	ArgsOnMouseWheel(MouseButtons::None, 0, 0, 0, 0),
	m_Enabled(true)
{

}

WinControl::~WinControl()
{
	DestroyWindow(static_cast<HWND>(Handle.ToPointer()));
}

// Static function which handle WinAPI messages to corresponding member function of the control
LRESULT WINAPI WinControl::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WinControl* const pWnd = static_cast<WinControl*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data do store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message function to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WinControl::HandleMessageForwarder));

		// Forward message to window class member function
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* This function is the savior...
Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
LRESULT WINAPI WinControl::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept
{
	// Retrieve pointer to window class
	WinControl* const pWnd = reinterpret_cast<WinControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->HandleMessage(hWnd, msg, lParam, wParam);
}

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI WinControl::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_SETCURSOR:
	{
		/* If an application processes this message, it should return TRUE to halt further processing or FALSE to continue. */
		return OnSetCursor_Impl(hWnd, (HWND)wParam, static_cast<unsigned int>(LOWORD(lParam)), static_cast<unsigned int>(HIWORD(lParam)));
	}
	case WM_CREATE:
	{
		OnCreate_Impl(hWnd, (LPCREATESTRUCT)lParam);
		break;
	}
	case WM_ERASEBKGND:
	{
		return OnEraseBackground_Impl(hWnd, (HDC)wParam);
	}
	case WM_COMMAND:
	{
		OnCommand_Impl(hWnd, static_cast<int>(LOWORD(wParam)), (HWND)lParam, static_cast<unsigned int>(HIWORD(wParam)));
		break;
	}
	case WM_CLOSE:
	{
		return OnClosing_Impl(hWnd);
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

	/******************** KEYBOARD MESSAGES *********************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:	// Syskey commands need to be handled to track ALT key (VK_MENU)
	{
		OnKeyDown_Impl(hWnd, static_cast<unsigned int>(wParam), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:	// Syskey commands need to be handled to track ALT key (VK_MENU)
	{
		OnKeyUp_Impl(hWnd, static_cast<unsigned int>(wParam), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
		break;
	}
	case WM_CHAR:
	{
		OnKeyPressed_Impl(hWnd, static_cast<char>(wParam), (int)(short)LOWORD(lParam));
		break;
	}
	/******************* END KEYBOARD MESSAGES ******************/

	case WM_GETDLGCODE:
	{
		return OnGetDLGCode(hWnd, (LPMSG)(lParam));
	}

	/********************** MOUSE MESSAGES **********************/
	case WM_MOUSEMOVE:
	{
		OnMouseMove_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		OnMouseLeftDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_MOUSELEAVE:
	{
		OnMouseLeave_Impl(hWnd);
		break;
	}
	case WM_LBUTTONUP:
	{
		OnMouseLeftUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		OnMouseRightDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_RBUTTONUP:
	{
		OnMouseRightUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		OnMouseLeftDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		OnMouseRightDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
		break;
	}
	case WM_MOUSEWHEEL:
	{
		OnMouseWheel_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam));
		break;
	}
	case WM_SHOWWINDOW:
	{
		OnShowWindow_Impl(hWnd, static_cast<bool>(wParam), static_cast<unsigned int>(lParam));
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
		OnRawInput_Impl(hWnd, GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam);
		break;
	}
	case WM_NEXTDLGCTL:
	{
		OnNextDialogControl_Impl(hWnd, (HWND)wParam, static_cast<bool>(lParam));
		break;
	}
	/****************** END RAW MOUSE MESSAGES ******************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

WinControl* WinControl::GetPreviousControl() noexcept
{
	if (const auto& root = dynamic_cast<WinControl*>(GetWindow()))
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

WinControl* WinControl::GetNextControl() noexcept
{
	if (const auto& root = dynamic_cast<WinControl*>(GetWindow()))
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

bool WinControl::IsEnabled() const noexcept
{
	if (!m_Enabled)
	{
		return false;
	}

	if (const auto& p = dynamic_cast<WinControl*>(Parent))
	{
		if (!p->IsEnabled())
		{
			return false;
		}
	}

	return true;
}

void WinControl::Enable() noexcept
{
	m_Enabled = true;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), true);
}

void WinControl::Disable() noexcept
{
	m_Enabled = false;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), false);
}

int WinControl::GetTabIndex() const noexcept
{
	return m_TabIndex;
}

void WinControl::SetTabIndex(const unsigned int& index) noexcept
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

void WinControl::Update()
{
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
}
