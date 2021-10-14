#pragma once

#include "NativeWindow.h"
#include "Application.h"
#include "Window.h"

// Linker for ::_TrackMouseEvent function (NativeWindow.cpp usage)
#pragma comment(lib, "comctl32.lib")
#include <Commctrl.h>

#include <sstream>

int NativeWindow::m_GlobalID = 1;
std::map<void*, int> NativeWindow::m_WindowMap = std::map<void*, int>();
std::map<short, void*> NativeWindow::m_HashForIdHandle = std::map<short, void*>();
std::map<void*, short> NativeWindow::m_HashForHandleId = std::map<void*, short>();
IntPtr NativeWindow::m_WindowProc = nullptr;
IntPtr NativeWindow::m_UserDefinedWindowProc = nullptr;
NativeWindow::WindowClass* NativeWindow::WindowClass::m_Cache = nullptr;
bool NativeWindow::m_AnyHandleCreated = false;
bool NativeWindow::m_AnyHandleCreatedInApp = false;

std::string NativeWindow::WindowClass::GetFullClassName(const std::string& className)
{
	std::ostringstream oss;

	oss << '.' << className << ".app.";

	return oss.str();
}

void NativeWindow::WindowClass::RegisterWindowClass()
{
	WNDCLASSEX wndClass;

	if (m_UserDefinedWindowProc == IntPtr::Zero())
	{
#if UNICODE
		const std::string defaultProc = "DefWindowProcW";
#else
		const std::string defaultProc = "DefWindowProcA";
#endif

		m_UserDefinedWindowProc = GetProcAddress(GetModuleHandle("user32.dll"), defaultProc.c_str());
		if (m_UserDefinedWindowProc == IntPtr::Zero()) throw Win32Exception();
	}

	std::ostringstream localClassName;

	if (m_ClassName.empty())
	{

		// If we don't use a hollow brush here, Windows will "pre paint" us with COLOR_WINDOW which
		// creates a little bit if flicker.  This happens even though we are overriding wm_erasebackgnd.
		// Make this hollow to avoid all flicker.
		wndClass.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH); //(IntPtr)(NativeMethods.COLOR_WINDOW + 1);
		wndClass.style = m_ClassStyle;

		//defWindowProc = userDefWindowProc;
		localClassName << "Window." << "0x" << std::hex << m_ClassStyle;
		m_HashCode = 0;
	}

	else
	{
		WNDCLASSA wcls;
		bool ok = GetClassInfo(nullptr, m_ClassName.c_str(), &wcls);
		int error = GetLastError();

		if (!ok) throw Win32Exception(error, "InvalidWndClsName");

		wndClass.style = wcls.style;
		wndClass.cbClsExtra = wcls.cbClsExtra;
		wndClass.cbWndExtra = wcls.cbWndExtra;
		wndClass.hIcon = wcls.hIcon;
		wndClass.hCursor = wcls.hCursor;
		wndClass.hbrBackground = wcls.hbrBackground;
		wndClass.lpszMenuName = wcls.lpszMenuName;
		localClassName << m_ClassName;
		m_DefaultWindowProc = wcls.lpfnWndProc;
		m_HashCode = GetHashCode();
	}

	// Our static data is different for different app domains, so we include the app domain in with
	// our window class name.  This way our static table always matches what Win32 thinks.
	m_WindowClassName = GetFullClassName(localClassName.str());
	m_WindowProc = &HandleMessageSetup;
	wndClass.lpfnWndProc = (WNDPROC)m_WindowProc.ToPointer();
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.lpszClassName = m_WindowClassName.c_str();

	short atom = RegisterClassEx(&wndClass);

	if (atom == 0)
	{
		int err = GetLastError();

		if (err == ERROR_CLASS_ALREADY_EXISTS)
		{
			// Check to see if the window class window
			// proc points to DefWndProc.  If it does, then
			// this is a class from a rudely-terminated app domain
			// and we can safely reuse it.  If not, we've got
			// to throw.
			WNDCLASSA wcls;
			bool ok = GetClassInfo(GetModuleHandle(nullptr), m_WindowClassName.c_str(), &wcls);
			if (ok && wcls.lpfnWndProc == m_UserDefinedWindowProc)
			{
				// We can just reuse this class because we have marked it
				// as being a nop in another domain.  All we need to do is call SetClassLong.
				// Only one problem:  SetClassLong takes an HWND, which we don't have.  That leaves
				// us with some tricky business. First, try this the easy way and see
				// if we can simply unregister and re-register the class.  This might
				// work because the other domain shutdown would have posted WM_CLOSE to all
				// the windows of the class.
				if (UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr)))
				{
					// If this fails, we will always raise the first err above.  No sense exposing our twiddling.
					atom = RegisterClassEx(&wndClass);
				}
				else
				{
					// This is a little harder.  We cannot reuse the class because it is
					// already in use.  We must create a new class.  We bump our domain qualifier
					// here to account for this, so we only do this expensive search once for the
					// domain.  
					do
					{
						m_WindowClassName = GetFullClassName(localClassName.str());
						wndClass.lpszClassName = m_WindowClassName.c_str();
						atom = RegisterClassEx(&wndClass);
					} while (atom == 0 && GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
				}
			}
		}

		if (atom == 0)
		{
			m_WindowProc = nullptr;
			throw Win32Exception(err);
		}
	}

	m_IsRegistered = true;
}

void NativeWindow::WindowClass::UnregisterWindowClass()
{
	if (m_IsRegistered && UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr)))
	{
		m_WindowProc = nullptr;
		m_IsRegistered = false;
	}
}

void NativeWindow::WindowClass::DisposeCache()
{
	WindowClass* wc = m_Cache;

	while (wc != nullptr)
	{
		WindowClass* oldWc = wc;
		wc->UnregisterWindowClass();
		wc = wc->m_Next;
		SafeDelete(oldWc);
	}
}

NativeWindow::WindowClass* NativeWindow::WindowClass::Create(const std::string& className, int classStyle)
{
	WindowClass* wc = m_Cache;

	if (className.empty())
	{
		while (wc != nullptr && (!wc->m_ClassName.empty() || wc->m_ClassStyle != classStyle))
			wc = wc->m_Next;
	}
	else
	{
		while (wc != nullptr && className != wc->m_ClassName)
			wc = wc->m_Next;
	}

	if (wc == nullptr)
	{
		wc = new WindowClass(className, classStyle);
		wc->m_Next = m_Cache;
		m_Cache = wc;
	}
	else
	{
		if (!wc->m_IsRegistered) wc->RegisterWindowClass();
	}

	return wc;
}

// Window class declarations
NativeWindow::WindowClass::WindowClass(const std::string& className, int classStyle) noexcept
	:
	m_ClassName(className),
	m_ClassStyle(classStyle)
{
	RegisterWindowClass();
}

unsigned int NativeWindow::m_CurrentIndex = 1;
IntPtr NativeWindow::m_OpenedControl = nullptr;

void NativeWindow::ReleaseHandle(bool handleValid)
{
	if (Handle != IntPtr::Zero())
	{
		if (handleValid) UnSubClass();

		RemoveWindowFromTable(Handle, this);
		Handle = IntPtr::Zero();
		OnHandleChange();
	}
}

void NativeWindow::UnSubClass()
{
	bool finalizing = Handle != nullptr;

	// Don't touch if the current window proc is not ours.
	IntPtr currentWinPrc = GetWindowLong(Handle, GWLP_WNDPROC);
	if (m_WindowProc == currentWinPrc)
	{
		if (m_PreviousWindow == nullptr)
		{
#if _DEBUG
			subclassStatus = "Unsubclassing back to native defWindowProc";
#endif
			// If the defWindowProc points to a native window proc, previousWindow will
			// be null.  In this case, it is completely safe to assign defWindowProc
			// to the current wndproc.
			SetWindowLong(Handle, GWLP_WNDPROC, m_DefaultWindowProc.ToInt64());
		}
		else
		{
			if (finalizing)
			{
#if DEBUG
				subclassStatus = "Setting back to userDefWindowProc -- next chain is managed";
#endif
				// Here, we are finalizing and defWindowProc is pointing to a managed object.  We must assume
				// that the object defWindowProc is pointing to is also finalizing.  Why?  Because we're
				// holding a ref to it, and it is holding a ref to us.  The only way this cycle will
				// finalize is if no one else is hanging onto it.  So, we re-assign the window proc to
				// userDefWindowProc.
				SetWindowLong(Handle, GWLP_WNDPROC, m_UserDefinedWindowProc.ToInt64());
			}
			else
			{
#if DEBUG
				subclassStatus = "Setting back to next managed subclass object";
#endif
				// Here we are not finalizing so we use the windowProc for our previous window.  This may
				// DIFFER from the value we are currently storing in defWindowProc because someone may
				// have re-subclassed.
				SetWindowLong(Handle, GWLP_WNDPROC, m_PreviousWindow->m_WindowProc.ToInt64());
			}
		}
	}
	else
	{
		// Cutting the subclass chain anyway, even if we're not the last one in the chain
		// If the whole chain is all managed NativeWindow it doesnt matter, 
		// if the chain is not, then someone has been dirty and didn't clean up properly, too bad for them...

		// We will cut off the chain if we cannot unsubclass.
		// If we find previouswindow pointing to us, then we can let RemoveWindowFromTable reassign the
		// m_DefaultWindowProc pointers properly when this guy gets removed (thereby unsubclassing ourselves)
		if (m_NextWindow == nullptr || m_NextWindow->m_DefaultWindowProc != m_WindowProc)
		{
			// We didn't find it... let's unhook anyway and cut the chain... this prevents crashes
			SetWindowLong(Handle, GWLP_WNDPROC, m_UserDefinedWindowProc.ToInt64());
		}
#if DEBUG
		subclassStatus = "FORCE unsubclass -- we do not own the subclass";
#endif
	}
}

void NativeWindow::CheckReleased() const
{
	if (Handle != IntPtr::Zero()) throw InvalidOperationException("HandleAlreadyExists");
}

unsigned int NativeWindow::InitHash(IntPtr handle, int hashsize, unsigned int& seed, unsigned int& incr)
{
	// Hashcode must be positive.  Also, we must not use the sign bit, since
	// that is used for the collision bit.
	unsigned int hashcode = ((unsigned int)handle.GetHashCode()) & 0x7FFFFFFF;
	seed = (unsigned int)hashcode;
	// Restriction: incr MUST be between 1 and hashsize - 1, inclusive for
	// the modular arithmetic to work correctly.  This guarantees you'll
	// visit every bucket in the table exactly once within hashsize 
	// iterations.  Violate this and it'll cause obscure bugs forever.
	// If you change this calculation for h2(key), update putEntry too!
	incr = (unsigned int)(1 + (((seed >> 5) + 1) % ((unsigned int)hashsize - 1)));
	return hashcode;
}

// Default PreDraw function which is used to recalculate elements according to DataSource, number of elements, etc...
void NativeWindow::PreDraw(Graphics* const graphics)
{
	auto hwnd = static_cast<HWND>(Handle.ToPointer());

	if (m_HasFontChanged)
	{
		graphics->CreateFontObject(m_Font);
		auto hFont = graphics->GetElement(std::string(m_Font.ToString()));
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, true);
		m_HasFontChanged = false;
	}

	graphics->CreateSolidBrush(GetBackgroundColor());
	graphics->CreateSolidBrush(GetForeColor());
}

void NativeWindow::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{

}

void NativeWindow::PostDraw(Graphics* const graphics)
{

}

void NativeWindow::OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized)
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

void NativeWindow::OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify)
{

}

int NativeWindow::OnClosing_Impl(HWND hwnd)
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
			Application::Exit();

		}
		return 0;
	}

	return 1;
}

void NativeWindow::OnClosed_Impl(HWND hwnd)
{
	Dispatch("OnClosed", &ArgsOnClosed);
}

void NativeWindow::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	Handle = hwnd;
	m_Graphics = Graphics::Create(hwnd, m_Size);
	m_Graphics->CreateSolidBrush(m_BackgroundColor);
	m_Graphics->CreateSolidBrush(m_ForeColor);
	m_Graphics->CreateFontObject(m_Font);
	Dispatch("OnCreate", &ArgsDefault);
}

void NativeWindow::OnEnable_Impl(HWND hwnd, bool fEnable)
{

}

int NativeWindow::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	// 0 always redraw
	// Some controls like have 1 to avoid flickering on input

	return 0;
}

void NativeWindow::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus)
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

void NativeWindow::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus)
{
	Update();
	Dispatch("OnLostFocus", &ArgsDefault);
}

int NativeWindow::OnGetDLGCode_Impl(HWND hwnd, LPMSG msg)
{
	return 0;
}

void NativeWindow::OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnHorizontalScroll", &ArgsDefault);
}

void NativeWindow::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	ArgsOnKeyDown = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyDown", &ArgsOnKeyDown);
}

void NativeWindow::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat)
{
	ArgsOnKeyPressed = KeyPressEventArgs(c);
	Dispatch("OnKeyPress", &ArgsOnKeyPressed);
}

void NativeWindow::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	ArgsOnKeyUp = KeyEventArgs(static_cast<Keys>(vk));
	Dispatch("OnKeyUp", &ArgsOnKeyUp);
}


void NativeWindow::OnMouseLeave_Impl(HWND hwnd)
{
	SetClickingState(false);
	SetMouseOverState(false);
	Update();
	Dispatch("OnMouseLeave", &ArgsDefault);
}

void NativeWindow::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
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

void NativeWindow::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	SetMouseOverState(true);
	SetClickingState(true);
	Update();
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Left, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void NativeWindow::OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
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

void NativeWindow::OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDown = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseDown", &ArgsOnMouseDown);
}

void NativeWindow::OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseUp = MouseEventArgs(MouseButtons::Right, 1, 0, x, y);
	Dispatch("OnMouseUp", &ArgsOnMouseUp);
}

void NativeWindow::OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Left, 2, 0, x, y);
	Dispatch("OnDoubleClick", &ArgsDefault);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void NativeWindow::OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags)
{
	ArgsOnMouseDoubleClick = MouseEventArgs(MouseButtons::Right, 2, 0, x, y);
	Dispatch("OnMouseDoubleClick", &ArgsOnMouseDoubleClick);
}

void NativeWindow::OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys)
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

void NativeWindow::OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext)
{

}

int NativeWindow::OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys)
{
	return 1;
}

void NativeWindow::OnPaint_Impl(HWND hwnd)
{
	if (IsShown())
	{
		Drawing::Rectangle rect = Drawing::Rectangle(0, 0, m_Size.Width, m_Size.Height);

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

void NativeWindow::OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput)
{

}

int NativeWindow::OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg)
{
	return 0;
}

void NativeWindow::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy)
{
	m_Size.Width = cx;
	m_Size.Height = cy;

	if (m_Graphics) m_Graphics->UpdateSize(m_Size);

	Dispatch("OnResize", &ArgsDefault);
}

void NativeWindow::OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status)
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

void NativeWindow::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	Dispatch("OnVerticalScroll", &ArgsDefault);
}

NativeWindow::NativeWindow(int width, int height, int x, int y)
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
	m_BackgroundColor(Color::ControlBackground_Win11()),
	m_ForeColor(Color::Foreground()),
	m_HorizontalScrolling(0),
	m_VerticalScrolling(0),
	m_VerticalScrollingUnit(0),
	m_VerticalScrollPaging(0),
	m_Size(width, height),
	m_Location(x, y),
	m_HasFontChanged(true),
	m_Graphics(nullptr),
	m_PreviousWindow(nullptr),
	m_NextWindow(nullptr)
{

}

NativeWindow::~NativeWindow() noexcept(false)
{
	SafeDelete(m_Graphics);
	if (DestroyWindow(static_cast<HWND>(Handle.ToPointer())) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

// Static function which handle WinAPI messages to corresponding member function of the control
LRESULT WINAPI NativeWindow::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		NativeWindow* const pWnd = static_cast<NativeWindow*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data do store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message function to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NativeWindow::HandleMessageForwarder));

		// Forward message to window class member function
		return pWnd->Callback(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* This function is the savior...
Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
LRESULT WINAPI NativeWindow::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam)
{
	// Retrieve pointer to window class
	NativeWindow* const pWnd = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->Callback(hWnd, msg, lParam, wParam);
}

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI NativeWindow::Callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Message m = Message::Create(hWnd, msg, wParam, lParam);

	try
	{
		if (Handle != nullptr) WndProc(m);
		else DefWndProc(m);
	}
	catch (const Exception& e)
	{
		//OnThreadException(e);
	}

	if (msg == WM_NCDESTROY) ReleaseHandle(false);
	//if (msg == WM_UIUNSUBCLASS) ReleaseHandle(true);

	return m.Result;

	//switch (msg)
	//{
	//	case WM_SETCURSOR:
	//	{
	//		/* If an application processes this message, it should return TRUE to halt further processing or FALSE to continue. */
	//		return OnSetCursor_Impl(hWnd, (HWND)wParam, static_cast<unsigned int>(LOWORD(lParam)), static_cast<unsigned int>(HIWORD(lParam)));
	//	}
	//	case WM_CREATE:
	//	{
	//		OnCreate_Impl(hWnd, (LPCREATESTRUCT)lParam);
	//		break;
	//	}
	//	case WM_ERASEBKGND:
	//	{
	//		return OnEraseBackground_Impl(hWnd, (HDC)wParam);
	//	}
	//	case WM_COMMAND:
	//	{
	//		OnCommand_Impl(hWnd, static_cast<int>(LOWORD(wParam)), (HWND)lParam, static_cast<unsigned int>(HIWORD(wParam)));
	//		break;
	//	}
	//	case WM_CLOSE:
	//	{
	//		return OnClosing_Impl(hWnd);
	//	}
	//	case WM_NCDESTROY:
	//	{
	//		OnClosed_Impl(hWnd);
	//		break;
	//	}
	//	case WM_SETFOCUS:
	//	{
	//		OnFocusEnter_Impl(hWnd, (HWND)wParam);
	//		break;
	//	}
	//	case WM_KILLFOCUS:
	//	{
	//		OnFocusLeave_Impl(hWnd, (HWND)wParam);
	//		break;
	//	}
	//	case WM_ACTIVATE:
	//	{
	//		OnActivate_Impl(hWnd, static_cast<unsigned int>(LOWORD(wParam)), (HWND)(lParam), static_cast<bool> (HIWORD(wParam)));
	//		break;
	//	}
	//	case WM_SIZE:
	//	{
	//		OnSize_Impl(hWnd, (unsigned int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
	//		break;
	//	}
	//	/******************** KEYBOARD MESSAGES *********************/
	//	case WM_KEYDOWN:
	//	case WM_SYSKEYDOWN:	// Syskey commands need to be handled to track ALT key (VK_MENU)
	//	{
	//		OnKeyDown_Impl(hWnd, static_cast<unsigned int>(wParam), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
	//		break;
	//	}
	//	case WM_KEYUP:
	//	case WM_SYSKEYUP:	// Syskey commands need to be handled to track ALT key (VK_MENU)
	//	{
	//		OnKeyUp_Impl(hWnd, static_cast<unsigned int>(wParam), (int)(short)LOWORD(lParam), static_cast<unsigned int>(HIWORD(lParam)));
	//		break;
	//	}
	//	case WM_CHAR:
	//	{
	//		OnKeyPressed_Impl(hWnd, static_cast<char>(wParam), (int)(short)LOWORD(lParam));
	//		break;
	//	}
	//	/******************* END KEYBOARD MESSAGES ******************/

	//	case WM_GETDLGCODE:
	//	{
	//		return OnGetDLGCode_Impl(hWnd, (LPMSG)(lParam));
	//	}

	//	/********************** MOUSE MESSAGES **********************/
	//	case WM_MOUSEMOVE:
	//	{
	//		OnMouseMove_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_LBUTTONDOWN:
	//	{
	//		OnMouseLeftDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_MOUSELEAVE:
	//	{
	//		OnMouseLeave_Impl(hWnd);
	//		break;
	//	}
	//	case WM_LBUTTONUP:
	//	{
	//		OnMouseLeftUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_RBUTTONDOWN:
	//	{
	//		OnMouseRightDown_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_RBUTTONUP:
	//	{
	//		OnMouseRightUp_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_LBUTTONDBLCLK:
	//	{
	//		OnMouseLeftDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_RBUTTONDBLCLK:
	//	{
	//		OnMouseRightDoubleClick_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), static_cast<unsigned int>(wParam));
	//		break;
	//	}
	//	case WM_MOUSEWHEEL:
	//	{
	//		OnMouseWheel_Impl(hWnd, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam));
	//		break;
	//	}
	//	case WM_SHOWWINDOW:
	//	{
	//		OnShowWindow_Impl(hWnd, static_cast<bool>(wParam), static_cast<unsigned int>(lParam));
	//		break;
	//	}
	//	case WM_PAINT:
	//	{
	//		OnPaint_Impl(hWnd);
	//		break;
	//	}
	//	/******************** END MOUSE MESSAGES ********************/

	//	/******************** RAW MOUSE MESSAGES ********************/
	//	case WM_INPUT:
	//	{
	//		OnRawInput_Impl(hWnd, GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam);
	//		break;
	//	}
	//	case WM_NEXTDLGCTL:
	//	{
	//		OnNextDialogControl_Impl(hWnd, (HWND)wParam, static_cast<bool>(lParam));
	//		break;
	//	}
	//	/****************** END RAW MOUSE MESSAGES ******************/
	//	/********************* SCROLL MESSAGES **********************/
	//	case WM_HSCROLL:
	//	{
	//		OnHorizontalScrolling_Impl(hWnd, (HWND)lParam, (unsigned int)LOWORD(wParam), (int)(short)HIWORD(wParam));
	//		break;
	//	}
	//	case WM_VSCROLL:
	//	{
	//		OnVerticalScrolling_Impl(hWnd, (HWND)lParam, (unsigned int)LOWORD(wParam), (int)(short)HIWORD(wParam));
	//		break;
	//	}
	//}
	///******************* END SCROLL MESSAGES ********************/
	//return DefWindowProc(hWnd, msg, wParam, lParam);
}

unsigned int NativeWindow::GetId() const noexcept
{
	return m_Id;
}

void NativeWindow::SetMouseOverState(bool state) noexcept
{
	m_IsMouseOver = state;
}

void NativeWindow::SetClickingState(bool state) noexcept
{
	m_IsClicking = state;
}

void NativeWindow::WndProc(Message& m)
{
	DefWndProc(m);
}

void NativeWindow::OnHandleChange()
{
}

void NativeWindow::Dispose()
{
	Component::Dispose();

	if (m_Graphics != nullptr) m_Graphics->Dispose();
}

void NativeWindow::CreateHandle(CreateParams& cp)
{
	CheckReleased();
	WindowClass* windowClass = WindowClass::Create(cp.ClassName, cp.ClassStyle);

	// Why are we checking the handle again after calling CheckReleased()?  It turns                     
	// out the CLR will sometimes pump messages while we're waiting on the lock.  If
	// a message comes through (say a WM_ACTIVATE for the parent) which causes the 
	// handle to be created, we can try to create the handle twice for the same 
	// NativeWindow object. See bug for more details.
	if (Handle != IntPtr::Zero()) return;

	windowClass->m_TargetWindow = this;
	IntPtr createResult = IntPtr::Zero();
	int lastWin32Error = 0;

	IntPtr modHandle = GetModuleHandle(nullptr);

	if (cp.Caption.length() > SHRT_MAX)
	{
		cp.Caption = cp.Caption.substr(0, SHRT_MAX);
	}

	createResult = CreateWindowEx(cp.ExStyle, windowClass->m_WindowClassName.c_str(),
		cp.Caption.c_str(), cp.Style, cp.X, cp.Y, cp.Width, cp.Height, cp.Parent, nullptr,
		modHandle, cp.Param);

	lastWin32Error = GetLastError();

	//windowClass->m_TargetWindow = nullptr;

	if (createResult == IntPtr::Zero()) throw Win32Exception(lastWin32Error, "ErrorCreatingHandle");

	m_OwnHandle = true;
}

void NativeWindow::DefWndProc(Message& m)
{
	if (m_PreviousWindow == nullptr)
	{
		if (m_DefaultWindowProc == IntPtr::Zero())
		{

#if _DEBUG
			std::ostringstream oss;
			oss << "Can't find a default window procedure for message " << m.ToString() << " on class " << GetType().ToString() << " subclass status: " << subclassStatus;
			printf_s(oss.str().c_str());
#endif

			// At this point, there isn't much we can do.  There's a
			// small chance the following line will allow the rest of
			// the program to run, but don't get your hopes up.
			m.Result = (void*)DefWindowProc(m.hWnd, m.Msg, m.wParam, m.lParam);
			return;
		}
		m.Result = (void*)CallWindowProc((WNDPROC)m_DefaultWindowProc.ToPointer(), m.hWnd, m.Msg, m.wParam, m.lParam);
	}
	else
	{
		assert(m_PreviousWindow != this, "Looping in our linked list");
		m.Result = (void*)m_PreviousWindow->Callback(m.hWnd, m.Msg, m.wParam, m.lParam);
	}
}

Point NativeWindow::GetLocation() const noexcept
{
	return m_Location;
}

void NativeWindow::SetLocation(Point p) noexcept
{
	m_Location = p;
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void NativeWindow::SetLocation(int x, int y) noexcept
{
	m_Location = Point(x, y);
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

Size NativeWindow::GetSize() const noexcept
{
	return m_Size;
}

void NativeWindow::Resize(Size s)
{
	m_Size = s;
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

void NativeWindow::Resize(int width, int height)
{
	m_Size = Size(width, height);
	SetWindowPos(static_cast<HWND>(Handle.ToPointer()), nullptr, m_Location.X, m_Location.Y, m_Size.Width, m_Size.Height, SWP_NOZORDER);
}

bool NativeWindow::IsMouseOver() const noexcept
{
	return m_IsMouseOver;
}

bool NativeWindow::IsClicking() const noexcept
{
	return m_IsClicking;
}

bool NativeWindow::IsEnabled() const noexcept
{
	return m_Enabled;
}

void NativeWindow::Enable()
{
	m_Enabled = true;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), true);
}

void NativeWindow::Disable()
{
	m_Enabled = false;
	EnableWindow(static_cast<HWND>(Handle.ToPointer()), false);
}

void NativeWindow::Update() const
{
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
}

const Graphics* NativeWindow::CreateGraphics() const noexcept
{
	return m_Graphics;
}

bool NativeWindow::IsShown() const noexcept
{
	return m_IsVisible;
}

void NativeWindow::Hide()
{
	if (IsShown())
	{
		m_IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void NativeWindow::Show()
{
	if (!IsShown())
	{
		m_IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}

Font NativeWindow::GetFont() const noexcept
{
	return m_Font;
}

void NativeWindow::SetFont(Font font) noexcept
{
	m_Font = font;

	m_HasFontChanged = true;
	Update();
}

Color NativeWindow::GetBackgroundColor() const noexcept
{
	if ((m_BackgroundColor == Color::ControlBackground_Win11() || m_BackgroundColor == Color::ControlBackground_Win10()) && !IsEnabled())
	{
		return Color::DisabledControlBackground();
	}

	return m_BackgroundColor;
}

void NativeWindow::SetBackgroundColor(const Color& color) noexcept
{
	m_BackgroundColor = color;
	Update();
}

Color NativeWindow::GetForeColor() const noexcept
{
	if (!IsEnabled()) return Color::DisabledForeground();
	return m_ForeColor;
}

void NativeWindow::SetForeColor(const Color& color) noexcept
{
	m_ForeColor = color;
	Update();
}

void NativeWindow::AddWindowToTable(IntPtr handle, NativeWindow* window)
{
	assert(handle != IntPtr::Zero(), "Should never insert a zero handle into the hash");

	// set a flag that this thread is tracking an HWND
	//
	m_AnyHandleCreated = true;
	// ...same for the application
	m_AnyHandleCreatedInApp = true;

	unsigned int seed;
	unsigned int incr;
	// Assume we only have one thread writing concurrently.  Modify
	// buckets to contain new data, as long as we insert in the right order.
	unsigned int hashcode = InitHash(handle, static_cast<int>(m_WindowMap.size()), seed, incr);

	int ntry = 0;
	int emptySlotNumber = -1; // We use the empty slot number to cache the first empty slot. We chose to reuse slots
	// create by remove that have the collision bit set over using up new slots.

	m_WindowMap[handle.ToPointer()] = hashcode;
}

void NativeWindow::RemoveWindowFromTable(IntPtr handle, NativeWindow* window)
{
	assert(handle != IntPtr::Zero(), "Incorrect handle");
	
	unsigned int seed;
	unsigned int incr;
	// Assuming only one concurrent writer, write directly into buckets.
	unsigned hashcode = InitHash(handle, static_cast<int>(m_WindowMap.size()), seed, incr);
	int ntry = 0;
	NativeWindow* prevWindow = window->m_PreviousWindow;
	
	if (window->m_PreviousWindow != nullptr)
	{
		window->m_PreviousWindow->m_NextWindow = window->m_NextWindow;
	}
	
	if (window->m_NextWindow != nullptr)
	{
		window->m_NextWindow->m_DefaultWindowProc = window->m_DefaultWindowProc;
		window->m_NextWindow->m_PreviousWindow = window->m_PreviousWindow;
	}
	
	m_WindowMap.erase(handle.ToPointer());
	SafeDelete(window);
}

void NativeWindow::AddWindowToIDTable(IntPtr handle)
{
	m_HashForIdHandle[m_GlobalID] = handle.ToPointer();
	m_HashForHandleId[handle.ToPointer()] = m_GlobalID;
	SetWindowLong(static_cast<HWND>(handle.ToPointer()), GWLP_ID, m_GlobalID);
	++m_GlobalID;
}

void NativeWindow::RemoveWindowFromIDTable(IntPtr handle)
{
	short id = m_HashForHandleId[handle.ToPointer()];
	m_HashForHandleId.erase(handle.ToPointer());
	m_HashForIdHandle.erase(id);
}
