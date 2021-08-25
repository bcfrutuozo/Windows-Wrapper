#pragma once

#include "WinAPI.h"
#include "Window.h"

unsigned int WinAPI::m_CurrentIndex = 1;

void WinAPI::OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized) noexcept
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

void WinAPI::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept
{

}

int WinAPI::OnClosing_Impl(HWND hwnd) noexcept
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

void WinAPI::OnClosed_Impl(HWND hwnd) noexcept
{
	Dispatch("OnClosed", &ArgsOnClosed);
}

void WinAPI::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) noexcept
{
	Dispatch("OnCreate", &ArgsDefault);
}

void WinAPI::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int WinAPI::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	// 0 always redraw
	// Some controls like have 1 to avoid flickering on input

	return 0;
}

void WinAPI::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
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
	Update();
	Dispatch("OnGotFocus", &ArgsDefault);
}

void WinAPI::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	Update();
	Dispatch("OnLostFocus", &ArgsDefault);
}

int WinAPI::OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept
{
	return 0;
}

void WinAPI::OnInitMenu_Impl(HWND hwnd, HMENU hMenu) noexcept
{

}

void WinAPI::OnInitMenuPopup_Impl(HWND hwnd, HMENU hMenu, unsigned int item, bool fSystemMenu) noexcept
{

}

void WinAPI::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	ArgsOnKeyDown = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyDown", &ArgsOnKeyDown);
}

void WinAPI::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	ArgsOnKeyPressed = KeyPressEventArgs(c);
	Dispatch("OnKeyPress", &ArgsOnKeyPressed);
}

void WinAPI::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	ArgsOnKeyUp = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyUp", &ArgsOnKeyUp);
}

int WinAPI::OnMenuChar_Impl(HWND hwnd, unsigned int ch, unsigned int flags, HMENU hmenu) noexcept
{
	/*
		MNC_CLOSE 1
		Informs the system that it should close the active menu.
		MNC_EXECUTE 2
		Informs the system that it should choose the item specified in the low - order word of the return value.The owner window receives a WM_COMMAND message.
		MNC_IGNORE 	0
		Informs the system that it should discard the character the user pressed and create a short beep on the system speaker.
		MNC_SELECT 	3
		Informs the system that it should select the item specified in the low - order word of the return value.
	*/
	return 0;
}

void WinAPI::OnMenuSelect_Impl(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, unsigned int flags) noexcept
{

}

void WinAPI::OnMouseLeave_Impl(HWND hwnd) noexcept
{
	SetClickingState(false);
	SetMouseOverState(false);
	Update();
	Dispatch("OnMouseLeave", &ArgsDefault);
}

void WinAPI::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
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

void WinAPI::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	SetMouseOverState(true);
	SetClickingState(true);
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinAPI::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
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

void WinAPI::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinAPI::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void WinAPI::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Left, 2, 0, x, y);
	Dispatch("OnDoubleClick", &ArgsDefault);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinAPI::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Right, 2, 0, x, y);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinAPI::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys) noexcept
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

void WinAPI::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept
{

}

int WinAPI::OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys) noexcept
{
	return 1;
}

void WinAPI::OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput) noexcept
{

}

int WinAPI::OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg) noexcept
{
	return 0;
}

void WinAPI::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept
{

}

void WinAPI::OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status) noexcept
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

WinAPI::WinAPI(WinAPI* parent, const std::string& text, int width, int height) noexcept
	:
	Parent(parent),
	Text(text),
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
	m_Enabled(true),
	m_Id(m_CurrentIndex++),
	m_IsMouseOver(false),
	m_IsClicking(false),
	m_Size(width, height)
{
	if (m_Size.Height == 0 || m_Size.Width == 0)
	{
		m_Size = CalculateSizeByFont();
	}
}

WinAPI::~WinAPI()
{
	DestroyWindow(static_cast<HWND>(Handle.ToPointer()));
}

// Static function which handle WinAPI messages to corresponding member function of the control
LRESULT WINAPI WinAPI::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WinAPI* const pWnd = static_cast<WinAPI*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data do store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message function to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WinAPI::HandleMessageForwarder));

		// Forward message to window class member function
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* This function is the savior...
Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
LRESULT WINAPI WinAPI::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept
{
	// Retrieve pointer to window class
	WinAPI* const pWnd = reinterpret_cast<WinAPI*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->HandleMessage(hWnd, msg, lParam, wParam);
}

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI WinAPI::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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
	/********************** MENU MESSAGES ***********************/
	case WM_INITMENU:
	{
		OnInitMenu_Impl(hWnd, (HMENU)wParam);
		break;
	}
	case WM_INITMENUPOPUP:
	{
		OnInitMenuPopup_Impl(hWnd, (HMENU)wParam, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam));
		break;
	}
	case WM_MENUSELECT:
	{
		OnMenuSelect_Impl(hWnd,
			(HMENU)lParam,
			(HIWORD(wParam) & MF_POPUP) ? 0L : (int)(LOWORD(wParam)),
			(HIWORD(wParam) & MF_POPUP) ? GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L,
			(unsigned int)((short)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam));
		break;
	}
	case WM_MENUCHAR:
	{
		OnMenuChar_Impl(hWnd, (unsigned int)LOWORD(wParam), (unsigned int)HIWORD(wParam), (HMENU)lParam);
		break;
	}
	/********************* END MENU MESSAGES ********************/

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

unsigned int WinAPI::GetId() const noexcept
{
	return m_Id;
}

void WinAPI::SetMouseOverState(bool state) noexcept
{
	m_IsMouseOver = state;
}

void WinAPI::SetClickingState(bool state) noexcept
{
	m_IsClicking = state;
}

Size WinAPI::CalculateSizeByFont() noexcept
{
	Size r(m_Size);

	if (m_Size.Height == 0)
	{
		int height = 5;
		for (int i = 5, j = 0; i < Font.GetSizeInPixels(); ++i, ++j)
		{
			if (j == 2)
			{
				height += 2;
				j = -1;
			}
			else
			{
				height += 1;
			}
		}

		// Size is always the default plus the calculated area size
		r.Height = GetControlDefaultHeight() + height;
	}

	if (m_Size.Width == 0)
	{
		SIZE s;
		HDC hdc = GetDC(static_cast<HWND>(GetWindow()->Handle.ToPointer()));
		GetTextExtentPoint32(hdc, Text.c_str(), Text.length(), &s);
		ReleaseDC(static_cast<HWND>(GetWindow()->Handle.ToPointer()), hdc);
		DeleteDC(hdc);
		r.Width = s.cx;
	}

	return r;
}

inline unsigned int WinAPI::GetControlDefaultHeight() const noexcept
{
	return 9; // Default size is 9 for 1 pixel font
}

bool WinAPI::IsEnabled() const noexcept
{
	if (!m_Enabled)
	{
		return false;
	}

	if (const auto& p = dynamic_cast<WinAPI*>(Parent))
	{
		if (!p->IsEnabled())
		{
			return false;
		}
	}

	return true;
}

Window* WinAPI::GetWindow() noexcept
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

bool WinAPI::IsMouseOver() const noexcept
{
	return m_IsMouseOver;
}

bool WinAPI::IsClicking() const noexcept
{
	return m_IsClicking;
}

void WinAPI::Enable() noexcept
{
	m_Enabled = true;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), true);
}

void WinAPI::Disable() noexcept
{
	m_Enabled = false;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), false);
}

void WinAPI::Update()
{
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
}
