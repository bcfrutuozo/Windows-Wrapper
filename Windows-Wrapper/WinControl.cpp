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
		Dispatch("OnActivate", new EventArgs());
	}
	else
	{
		Dispatch("OnDeactivate", new EventArgs());
	}
}

void WinControl::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept
{

}

int WinControl::OnClosing_Impl(HWND hwnd) noexcept
{
	bool cancel = false;
	Dispatch("OnClosing", new OnClosingEventArgs(CloseReason::UserClosing, cancel));

	// If cancel is false and doesn't have Parent Control, means that it's the main window
	// and application must post quit
	if (!cancel && Parent == nullptr)
	{
		PostQuitMessage(0);
	}

	return 0;
}

void WinControl::OnClosed_Impl(HWND hwnd) noexcept
{
	Dispatch("OnClosed", new OnClosedEventArgs(CloseReason::UserClosing));
}

HBRUSH WinControl::OnColorControl_Impl(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
	return CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
}

void WinControl::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) noexcept
{
	Dispatch("OnCreate", new EventArgs());
}

void WinControl::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int WinControl::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	// 0 always redraw
	// Some controls like textbox have 1 to avoid flickering on input

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

		If the CausesValidation property is set to false, the Validatingand Validated events are
		suppressed.
	/**************************************************************************************************/

	Dispatch("OnGotFocus", new EventArgs());
}

void WinControl::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	Dispatch("OnLostFocus", new EventArgs());
}

int WinControl::OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept
{
	return 0;
}

void WinControl::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	Dispatch("OnKeyDown", new KeyEventArgs(static_cast<Keys>(vk)));
}

void WinControl::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	Dispatch("OnKeyPress", new KeyPressEventArgs(c));
}

void WinControl::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	Dispatch("OnKeyUp", new KeyEventArgs(static_cast<Keys>(vk)));
}

void WinControl::OnMove_Impl(HWND hwnd, int x, int y) noexcept
{

}

void WinControl::OnMouseLeave_Impl(HWND hwnd) noexcept
{
	SetClickingState(false);
	SetMouseOverState(false);
	InvalidateRect(hwnd, nullptr, TRUE);
	Dispatch("OnMouseLeave", new EventArgs());
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
		Dispatch("OnMouseEnter", new EventArgs());
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT); //Monitor mouse to leave
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = hwnd;
	if (::_TrackMouseEvent(&tme))
	{
		SetMouseOverState(true);
		InvalidateRect(hwnd, nullptr, TRUE);
	}

	Dispatch("OnMouseMove", new MouseEventArgs(MouseButtons::None, 0, 0, x, y));
}

void WinControl::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	SetMouseOverState(true);
	SetClickingState(true);
	InvalidateRect(hwnd, nullptr, TRUE);
	Dispatch("OnMouseDown", new MouseEventArgs(MouseButtons::Left, 1, 0, x, y));
}

void WinControl::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	SetClickingState(false);
	SetMouseOverState(true);
	InvalidateRect(hwnd, nullptr, TRUE);
	Dispatch("OnClick", new EventArgs());
	Dispatch("OnMouseClick", new MouseEventArgs(MouseButtons::Left, 1, 0, x, y));
	Dispatch("OnMouseUp", new MouseEventArgs(MouseButtons::Left, 1, 0, x, y));
}

void WinControl::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	Dispatch("OnMouseDown", new MouseEventArgs(MouseButtons::Right, 1, 0, x, y));
}

void WinControl::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	Dispatch("OnMouseUp", new MouseEventArgs(MouseButtons::Right, 1, 0, x, y));
}

void WinControl::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	Dispatch("OnDoubleClick", new EventArgs());
	Dispatch("OnMouseDoubleClick", new MouseEventArgs(MouseButtons::Left, 2, 0, x, y));
}

void WinControl::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	Dispatch("OnMouseDoubleClick", new MouseEventArgs(MouseButtons::Right, 2, 0, x, y));
}

void WinControl::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys) noexcept
{
	switch (fwKeys)
	{
	case MK_LBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Left, 0, delta, x, y)); break;
	case MK_MBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Middle, 0, delta, x, y)); break;
	case MK_RBUTTON: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::Right, 0, delta, x, y));  break;
	case MK_XBUTTON1: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::XButton1, 0, delta, x, y)); break;
	case MK_XBUTTON2: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::XButton2, 0, delta, x, y)); break;
	default: Dispatch("OnMouseWheel", new MouseEventArgs(MouseButtons::None, 0, delta, x, y)); break;
	}
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
		Dispatch("OnShown");
	}
	else
	{
		Dispatch("OnHide");
	}

	Dispatch("OnVisibleChanged", new EventArgs());
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
			if (ret != nullptr)
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
	m_TabIndex(m_IncrementalTabIndex++)
{

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
		int searchIndex = m_TabIndex == 0 ? m_IncrementalTabIndex : m_TabIndex - 1;

		for (int i = searchIndex; i >= 0; --i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr)
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
		int searchIndex = m_TabIndex > m_IncrementalTabIndex ? 0 : m_TabIndex + 1;

		for (int i = searchIndex; i <= m_IncrementalTabIndex; ++i)
		{
			const auto& ret = root->GetByTabIndex(i);
			if (ret != nullptr)
			{
				return ret;
			}
		}
	}

	// Returning nullptr is extremely important, otherwise it will be a trash pointer and will launch an exception trying to process it
	return nullptr;
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
