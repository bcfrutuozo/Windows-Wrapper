#pragma once

#include "WinAPI.h"
#include "Application.h"
#include "Window.h"
#include "GDI.h"
#include "Direct2D.h"

std::map<std::string, HFONT>* WinAPI::Fonts = new std::map<std::string, HFONT>();
unsigned int WinAPI::m_CurrentIndex = 1;
IntPtr WinAPI::m_OpenedControl = nullptr;

// Default PreDraw function which is used to recalculate elements according to DataSource, number of elements, etc...
void WinAPI::PreDraw(Graphics* const graphics)
{
	auto hwnd = static_cast<HWND>(Handle.ToPointer());
	auto hdc = static_cast<HDC>(graphics->GetHDC().ToPointer());

	auto hFont = Fonts->find(m_Font.ToString());

	if (m_HasFontChanged)
	{
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont->second, TRUE);
		m_HasFontChanged = false;
	}

	SelectObject(hdc, hFont->second);
}

void WinAPI::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{

}

void WinAPI::PostDraw(Graphics* const graphics)
{

}

void WinAPI::OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized)
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

void WinAPI::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify)
{

}

int WinAPI::OnClosing_Impl(HWND hwnd)
{
	Dispatch("OnClosing", &ArgsOnClosing);

	// If cancel is false and doesn't have Parent Control, means that it's the main window
	// and application must post quit
	if (!ArgsOnClosing.Cancel)
	{
		if (GetType() == typeid(Window))
		{
			Dispose();
			Application::RemoveWindow(dynamic_cast<Window*>(this));
		}

		if (Application::CanCloseApplication())
		{
			WinAPI::Free();
			Application::Exit();

		}
		return 0;
	}

	return 1;
}

void WinAPI::OnClosed_Impl(HWND hwnd)
{
	Dispatch("OnClosed", &ArgsOnClosed);
}

void WinAPI::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	if (!Fonts->contains(m_Font.ToString()))
	{
		HFONT hFont = CreateFont(
			m_Font.GetSizeInPixels(),
			0,
			0,
			0,
			m_Font.IsBold() ? FW_BOLD : FW_NORMAL,
			m_Font.IsItalic(),
			m_Font.IsUnderline(),
			m_Font.IsStrikeOut(),
			ANSI_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			m_Font.GetName().c_str());

		(*Fonts)[m_Font.ToString()] = hFont;
	}

	Dispatch("OnCreate", &ArgsDefault);
}

void WinAPI::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int WinAPI::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	// 0 always redraw
	// Some controls like have 1 to avoid flickering on input

	return 0;
}

void WinAPI::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus)
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

void WinAPI::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus)
{
	Update();
	Dispatch("OnLostFocus", &ArgsDefault);
}

int WinAPI::OnGetDLGCode_Impl(HWND hwnd, LPMSG msg)
{
	return 0;
}

void WinAPI::OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnHorizontalScroll", &ArgsDefault);
}

void WinAPI::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	ArgsOnKeyDown = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyDown", &ArgsOnKeyDown);
}

void WinAPI::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat)
{
	ArgsOnKeyPressed = KeyPressEventArgs(c);
	Dispatch("OnKeyPress", &ArgsOnKeyPressed);
}

void WinAPI::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	ArgsOnKeyUp = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyUp", &ArgsOnKeyUp);
}


void WinAPI::OnMouseLeave_Impl(HWND hwnd)
{
	SetClickingState(false);
	SetMouseOverState(false);
	Update();
	Dispatch("OnMouseLeave", &ArgsDefault);
}

void WinAPI::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
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

	TRACKMOUSEEVENT tme = { 0 };
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

void WinAPI::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	SetMouseOverState(true);
	SetClickingState(true);
	Update();
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinAPI::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
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

void WinAPI::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void WinAPI::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void WinAPI::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Left, 2, 0, x, y);
	Dispatch("OnDoubleClick", &ArgsDefault);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinAPI::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Right, 2, 0, x, y);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void WinAPI::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys)
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

void WinAPI::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext)
{

}

int WinAPI::OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys)
{
	return 1;
}

void WinAPI::OnPaint_Impl(HWND hwnd)
{
	if (IsShown())
	{
		Drawing::Rectangle rect = Drawing::Rectangle(m_Location, m_Size);

		if (m_Graphics == nullptr)
		{
			m_Graphics = Graphics::Create(Handle, m_Size);
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

void WinAPI::OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput)
{

}

int WinAPI::OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg)
{
	return 0;
}

void WinAPI::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy)
{
	Dispatch("OnResize", &ArgsDefault);
}

void WinAPI::OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status)
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

void WinAPI::Free()
{
	for (const auto& f : *Fonts)
	{
		DeleteObject(f.second);
	}

	Fonts->clear();
	SafeDelete(Fonts);
}

void WinAPI::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnVerticalScroll", &ArgsDefault);
}

WinAPI::WinAPI(int width, int height, int x, int y)
	:
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
	m_Font("Segoe", 9, false, false, false, false, GraphicsUnit::Point),		// Default application font for controls
	m_Enabled(true),
	m_Id(m_CurrentIndex++),
	m_IsMouseOver(false),
	m_IsClicking(false),
	m_IsVisible(true),
	m_BackgroundColor(Color::ControlBackground()),
	m_ForeColor(Color::Foreground()),
	m_HorizontalScrolling(0),
	m_VerticalScrolling(0),
	m_VerticalScrollingUnit(0),
	m_VerticalScrollPaging(0),
	m_Size(width, height),
	m_Location(x, y),
	m_HasFontChanged(true),
	m_Graphics(nullptr)
{

}

WinAPI::~WinAPI() noexcept(false)
{
	SafeDelete(m_Graphics);
	if (DestroyWindow(static_cast<HWND>(Handle.ToPointer())) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

// Static function which handle WinAPI messages to corresponding member function of the control
LRESULT WINAPI WinAPI::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
LRESULT WINAPI WinAPI::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam)
{
	// Retrieve pointer to window class
	WinAPI* const pWnd = reinterpret_cast<WinAPI*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->HandleMessage(hWnd, msg, lParam, wParam);
}

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI WinAPI::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#if _DEBUG
	//printf_s(m(msg, wParam, lParam).c_str());
#endif

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
		case WM_SIZE:
		{
			OnSize_Impl(hWnd, (unsigned int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
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
			return OnGetDLGCode_Impl(hWnd, (LPMSG)(lParam));
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

void WinAPI::Dispose()
{
	Component::Dispose();

	if(m_Graphics != nullptr) m_Graphics->Dispose();
}

Point WinAPI::GetLocation() const noexcept
{
	return m_Location;
}

void WinAPI::SetLocation(Point p) noexcept
{
	m_Location = p;
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void WinAPI::SetLocation(int x, int y) noexcept
{
	m_Location = Point(x, y);
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

Size WinAPI::GetSize() const noexcept
{
	return m_Size;
}

void WinAPI::Resize(Size s) noexcept
{
	m_Size = s;
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void WinAPI::Resize(int width, int height) noexcept
{
	m_Size = Size(width, height);
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

bool WinAPI::IsMouseOver() const noexcept
{
	return m_IsMouseOver;
}

bool WinAPI::IsClicking() const noexcept
{
	return m_IsClicking;
}

bool WinAPI::IsEnabled() const noexcept
{
	return m_Enabled;
}

void WinAPI::Enable()
{
	m_Enabled = true;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), true);
}

void WinAPI::Disable()
{
	m_Enabled = false;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), false);
}

void WinAPI::Update() const
{
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
}

const Graphics* WinAPI::CreateGraphics() const noexcept
{
	return m_Graphics;
}

bool WinAPI::IsShown() const noexcept
{
	return m_IsVisible;
}

void WinAPI::Hide()
{
	if (IsShown())
	{
		m_IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void WinAPI::Show()
{
	if (!IsShown())
	{
		m_IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}

Font WinAPI::GetFont() const noexcept
{
	return m_Font;
}

void WinAPI::SetFont(Font font) noexcept
{
	if (!Fonts->contains(font.ToString()))
	{
		HFONT hFont = CreateFont(
			font.GetSizeInPixels(),
			0,
			0,
			0,
			font.IsBold() ? FW_BOLD : FW_NORMAL,
			font.IsItalic(),
			font.IsUnderline(),
			font.IsStrikeOut(),
			ANSI_CHARSET,
			OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			font.GetName().c_str());

		(*Fonts)[font.ToString()] = hFont;
	}

	m_Font = font;

	m_HasFontChanged = true;
	Update();
}

Color WinAPI::GetBackgroundColor() const noexcept
{
	if (m_BackgroundColor == Color::ControlBackground() && !IsEnabled())
	{
		return Color::DisabledControlBackground();
	}

	return m_BackgroundColor;
}

void WinAPI::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
	Update();
}

Color WinAPI::GetForeColor() const noexcept
{
	if (!IsEnabled()) return Color::DisabledForeground();
	return m_ForeColor;
}

void WinAPI::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
	Update();
}