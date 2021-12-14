#include "Window.h"
#include "OnClosedEventHandler.h"
#include "OnClosingEventHandler.h"
#include "Application.h"
#include "SystemInformation.h"

Window::Window()
	:
	ContainerControl(),
	m_IsCursorEnabled(true),
	m_Keyboard(std::make_unique<Keyboard>()),
	m_Mouse(std::make_unique<Mouse>()),
	m_IsMenuStripEnabled(false),
	m_FormBorderStyle(FormBorderStyle::Sizable),
	m_FormWindowState(FormWindowState::Normal),
	m_FormStartPosition(FormStartPosition::WindowsDefaultLocation),
	m_HasControlBox(true),
	m_HasMaximizeBox(true),
	m_HasMinimizeBox(true),
	m_HasHelpButton(false),
	m_IsLayered(false),
	m_HasTaskBar(true),
	m_IsIconShown(true),
	m_FormStateMdiChildMax(0),
	m_FormStateSetClientSize(0),
	m_FormStateShowWindowOnCreate(false),
	m_RestoredWindowBounds(-1, -1, -1, -1),
	m_RestoreBounds(-1, -1, -1, -1),
	m_IsMDI(false),
	m_MDIParent(nullptr),
	m_CtlClient(nullptr)
{
	SetState(STATE_VISIBLE, false);
	SetState(STATE_TOPLEVEL, true);
	CreateHandle();
}

void Window::OnClosingSet(const std::function<void(Object*, OnClosingEventArgs*)>& callback) noexcept
{
	Events.Register(new OnClosingEventHandler("OnClosing", callback));
}

void Window::OnShownSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	Events.Register(new EventHandler("OnShown", callback));
}

void Window::FillInCreateParamsBorderStyles(CreateParams* cp)
{
	switch(m_FormBorderStyle)
	{
		case FormBorderStyle::FixedSingle:
		{
			cp->Style |= WS_BORDER;
			break;
		}
		case FormBorderStyle::Sizable:
		{
			cp->Style |= WS_BORDER | WS_THICKFRAME;
			break;
		}
		case FormBorderStyle::Fixed3D:
		{
			cp->Style |= WS_BORDER;
			cp->ExStyle |= WS_EX_CLIENTEDGE;
			break;
		}
		case FormBorderStyle::FixedDialog:
		{
			cp->Style |= WS_BORDER;
			cp->ExStyle |= WS_EX_DLGMODALFRAME;
			break;
		}
		case FormBorderStyle::FixedToolWindow:
		{
			cp->Style |= WS_BORDER;
			cp->ExStyle |= WS_EX_TOOLWINDOW;
			break;
		}
		case FormBorderStyle::SizableToolWindow:
		{
			cp->Style |= WS_BORDER | WS_THICKFRAME;
			cp->ExStyle |= WS_EX_TOOLWINDOW;
			break;
		}
	}
}

void Window::FillInCreateParamsWindowState(CreateParams* cp)
{
	switch(m_FormWindowState)
	{
		case FormWindowState::Maximized:
			cp->Style |= WS_MAXIMIZE;
			break;
		case FormWindowState::Minimized:
			cp->Style |= WS_MINIMIZE;
			break;
	}
}

void Window::FillInCreateParamsBorderIcons(CreateParams* cp)
{
	if(m_FormBorderStyle != FormBorderStyle::None)
	{
		if(Text.length() > 0)
		{
			cp->Style |= WS_CAPTION;
		}

		if(m_HasControlBox)
		{
			cp->Style |= WS_SYSMENU | WS_CAPTION;
		}
		else
		{
			cp->Style &= (~WS_SYSMENU);
		}

		if(m_HasMaximizeBox)
		{
			cp->Style |= WS_MAXIMIZEBOX;
		}
		else
		{
			cp->Style &= ~WS_MAXIMIZEBOX;
		}

		if(m_HasMinimizeBox)
		{
			cp->Style |= WS_MINIMIZEBOX;
		}
		else
		{
			cp->Style &= ~WS_MINIMIZEBOX;
		}

		if(m_HasHelpButton && !m_HasMaximizeBox && !m_HasMinimizeBox && m_HasControlBox)
		{
			cp->ExStyle |= WS_EX_CONTEXTHELP;
		}
		else
		{
			cp->ExStyle &= ~WS_EX_CONTEXTHELP;
		}
	}
}

void Window::FillInCreateParamsStartPosition(CreateParams* cp)
{
	if(m_FormStateSetClientSize != 0)
	{
		int maskedStyle = cp->Style & ~(WS_MAXIMIZE | WS_MINIMIZE);
		Size correct = ComputeWindowSize(GetSize(), maskedStyle, cp->ExStyle);

		cp->Width = correct.Width;
		cp->Height = correct.Height;
	}

	switch(m_FormStartPosition)
	{
		case FormStartPosition::WindowsDefaultBounds:
			cp->Width = CW_USEDEFAULT;
			cp->Height = CW_USEDEFAULT;	// NO BREAK!
		case FormStartPosition::WindowsDefaultLocation:
		case FormStartPosition::CenterParent:
		{
			//if(IsMDIChild() && m_DockStyle::None != Dock)
			//{
			//	break;
			//}
			//cp.X = CW_USEDEFAULT;
			//cp.Y = CW_USEDEFAULT;
			//break;
		}
		case FormStartPosition::CenterScreen:
			if(IsMDIChild())
			{
				//Control* mdiclient = MdiParentInternal->MdiClient;
				//Drawing::Rectangle clientRect = mdiclient->ClientRectangle;
				//
				//cp.X = Math.Max(clientRect.X, clientRect.X + (clientRect.Width - cp.Width) / 2);
				//cp.Y = Math.Max(clientRect.Y, clientRect.Y + (clientRect.Height - cp.Height) / 2);
			}
			else
			{
					

				Drawing::Rectangle screenRect(0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
				//if, we're maximized, then don't set the x & y coordinates (they're @ (0,0) )
				if(m_FormWindowState != FormWindowState::Maximized)
				{
					cp->X = (std::max)(screenRect.X, screenRect.X + (screenRect.Width - cp->Width) / 2);
					cp->Y = (std::max)(screenRect.Y, screenRect.Y + (screenRect.Height - cp->Height) / 2);
				}
			}
			break;
	}
}

CreateParams* Window::CreateParameters()
{
	CreateParams* cp = Control::CreateParameters();

	if(IsHandleCreated() && (GetWindowStyle() & WS_DISABLED) != 0)
	{
		// Forms that are parent of a modal dialog must keep their WS_DISABLED style (VSWhidbey 449309)
		cp->Style |= WS_DISABLED;
	}
	else if(IsTopLevel())
	{
		// It doesn't seem to make sense to allow a top-level form to be disabled
		cp->Style &= (~WS_DISABLED);
	}

	if(IsTopLevel() && m_IsLayered != 0)
	{
		cp->ExStyle |= WS_EX_LAYERED;
	}

	cp->Parent = Parent;

	FillInCreateParamsBorderStyles(cp);
	FillInCreateParamsWindowState(cp);
	FillInCreateParamsBorderIcons(cp);

	if(m_HasTaskBar != 0)
	{
		cp->ExStyle |= WS_EX_APPWINDOW;
	}

	if(!IsIconShown() && (m_FormBorderStyle == FormBorderStyle::Sizable ||
						  m_FormBorderStyle == FormBorderStyle::Fixed3D ||
						  m_FormBorderStyle == FormBorderStyle::FixedSingle))
	{
		cp->ExStyle |= WS_EX_DLGMODALFRAME;
	}

	if(IsMDIChild())
	{
		if(IsVisible() && (m_FormWindowState == FormWindowState::Maximized || m_FormWindowState == FormWindowState::Normal))
		{
			Window* formMdiParent = m_MDIParent;
			//Form form = formMdiParent.ActiveMdiChildInternal;

			//if(form != nullptr && form->WindowState == FormWindowState::Maximized)
			//{
			//	cp.Style |= WS_MAXIMIZE;
			//	m_FormWindowState = FormWindowState::Maximized;
			//	//SetState(STATE_SIZELOCKEDBYOS, true);
			//}
		}

		if(m_FormStateMdiChildMax != 0)
		{
			cp->Style |= WS_MAXIMIZE;
		}
		cp->ExStyle |= WS_EX_MDICHILD;
	}

	if(IsTopLevel() || IsMDIChild())
	{
		FillInCreateParamsStartPosition(cp);
		// Delay setting to visible until after the handle gets created
		// to allow applyClientSize to adjust the size before displaying
		// the form.
		//
		if((cp->Style & WS_VISIBLE) != 0)
		{
			m_FormStateShowWindowOnCreate = true;
			cp->Style &= (~WS_VISIBLE);
		}
		else
		{
			m_FormStateShowWindowOnCreate = false;
		}
	}

	//if (RightToLeft == RightToLeft.Yes && RightToLeftLayout == true) {
	//	//We want to turn on mirroring for Form explicitly.
	//	cp.ExStyle |= WS_EX_LAYOUTRTL | WS_EX_NOINHERITLAYOUT;
	//	//Don't need these styles when mirroring is turned on.
	//	cp.ExStyle &= ~(WS_EX_RTLREADING | WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR);
	//}
	return cp;
}

void Window::OnClosedSet(const std::function<void(Object*, OnClosedEventArgs*)>& callback) noexcept
{
	Events.Register(new OnClosedEventHandler("OnClosing", callback));
}

//void Window::Initialize()
//{
//	// Calculate window size based on desired client region
//	//RECT r = { 0 };
//	//r.left = 100;
//	//r.right = m_Size.Width + r.left;
//	//r.top = 100;
//	//r.bottom = m_Size.Height + r.top;
//	//if(AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE, FALSE) == 0)
//	//{
//	//	throw CTL_LAST_EXCEPT();
//	//}
//
//	// Create window and get its handle
//	//CreateWindow(
//	//	"STATIC",																								// Class name
//	//	Text.c_str(),																										// Window title
//	//	WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,		// Style values
//	//	CW_USEDEFAULT,																										// X position
//	//	CW_USEDEFAULT,																										// Y position
//	//	(r.right - r.left),																									// Width
//	//	(r.bottom - r.top),																									// Height
//	//	nullptr,																											// Parent handle
//	//	nullptr,																											// Menu handle
//	//	nullptr,																							// Module instance handle
//	//	this																												// Pointer to the window instance to work along with HandleMessageSetup function.
//	//);
//	//
//	//if (Handle.IsNull())
//	//{
//	//	throw CTL_LAST_EXCEPT();
//	//}
//
//	// TODO: THIS IS WHERE THE GRAPHICS DEVICE, CONTEXT, RENDER TARGET, DEPTHSTENCIL (MAYBE) WILL BE INSTANTIATED.
//	// YES: THIS IS GONNA TAKE SOME TIME DO DESIGN
//	// AND YES AS WELL: WILL BE PROBABLY POORLY DONE :( 
//
//	// Register mouse raw input device
//	RAWINPUTDEVICE rid = { 0 };
//	rid.usUsagePage = 0x01; // Mouse page
//	rid.usUsage = 0x02; // Mouse page
//	rid.dwFlags = 0;
//	rid.hwndTarget = nullptr;
//	if(RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false)
//	{
//		throw CTL_LAST_EXCEPT();
//	}
//	// Still don't know how I'll implement child windows. So this is just to not let an empty ugly function
//	//SetParent(static_cast<HWND>(Handle.ToPointer()), static_cast<HWND>(Parent->Handle.ToPointer()));
//	
//	// Force window redraw to set Background color
//	m_BackgroundColor = Color::WindowBackground();
//	
//	// Add window the the application windows container
//	Application::AddWindow(this);
//	Update();
//}

void Window::ClearMenuStrip() noexcept
{
	//SetMenu(static_cast<HWND>(Handle.ToPointer()), NULL);

	//for (auto& c : Controls)
	//{
	//	if (c->GetType() == typeid(MenuBar))
	//	{
	//		c->Delete();
	//		const auto it = std::find(Controls.begin(), Controls.end(), c);
	//		Controls.erase(it);

	//		// Break processing to avoid exception.
	//		// We are removing an element inside a for loop which will cause the compiler to reference a null iterator.
	//		// And as we have just one MenuStrip per Window. It's a valid assumption.
	//		break;
	//	}
	//}
}

void Window::UpdateMenuStrip() noexcept
{
	/*for (auto& c : Controls)
	{
		if (c->GetType() == typeid(MenuBar))
		{
			SetMenu(static_cast<HWND>(Handle.ToPointer()), static_cast<HMENU>(c->Handle.ToPointer()));
			c->Update();
			m_IsMenuStripEnabled = true;
		}
	}*/
}

//MenuBar& Window::GetMenuStrip() noexcept
//{
//	for (auto& c : Controls)
//	{
//		if (c->GetType() == typeid(MenuBar))
//		{
//			return dynamic_cast<MenuBar&>(*c);
//		}
//	}
//
//	return Create<MenuBar>(this);
//}

void Window::SetText(const std::string& title)
{
	if(IsVisible())
	{
		if(SetWindowText(static_cast<HWND>(GetHandle().ToPointer()), title.c_str()) == 0)
		{
			throw CTL_LAST_EXCEPT();
		}

		Text = title;

		if(RedrawWindow(static_cast<HWND>(GetHandle().ToPointer()), NULL, NULL, RDW_INVALIDATE | RDW_ERASE) == 0)
		{
			throw CTL_LAST_EXCEPT();
		}
	}
}

void Window::EnableCursor() noexcept
{
	if(m_Mouse != nullptr)
	{
		m_IsCursorEnabled = true;
		ShowCursor();
		FreeCursor();
	}
}

void Window::DisableCursor() noexcept
{
	if(m_Mouse != nullptr)
	{
		m_IsCursorEnabled = false;
		HideCursor();
		EncloseCursor();
	}
}

Size Window::ComputeWindowSize(Size clientSize)
{
	CreateParams* cp = CreateParameters();
	return ComputeWindowSize(clientSize, cp->Style, cp->ExStyle);
}

Size Window::ComputeWindowSize(Size clientSize, int style, int exStyle)
{
	RECT result = RECT(0, 0, clientSize.Width, clientSize.Height);
	AdjustWindowRectEx(&result, style, HasMenu(), exStyle);
	return Size(result.right - result.left, result.bottom - result.top);
}

void Window::EncloseCursor() noexcept
{
	RECT r;
	GetClientRect(static_cast<HWND>(GetHandle().ToPointer()), &r);
	MapWindowPoints(static_cast<HWND>(GetHandle().ToPointer()), nullptr, reinterpret_cast<POINT*>(&r), 2);
	ClipCursor(&r);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
	while(::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor() noexcept
{
	while(::ShowCursor(TRUE) < 0);
}

bool Window::HasMenu() noexcept
{
	return false;
}

void Window::UpdateWindowState()
{
	if(IsHandleCreated())
	{
		FormWindowState oldState = WindowState();
		WINDOWPLACEMENT wp = { 0 };
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(GetHandle(), &wp);

		switch(wp.showCmd)
		{
			case SW_NORMAL:
			case SW_RESTORE:
			case SW_SHOW:
			case SW_SHOWNA:
			case SW_SHOWNOACTIVATE:
				if(m_FormWindowState != FormWindowState::Normal)
				{
					m_FormWindowState = FormWindowState::Normal;
				}
				break;
			case SW_SHOWMAXIMIZED:
				//if(m_FormWindowState == 0)
				//{
				//	m_FormWindowState = FormWindowState::Maximized;
				//}
				break;
			case SW_SHOWMINIMIZED:
			case SW_MINIMIZE:
			case SW_SHOWMINNOACTIVE:
				//if(formState[FormStateMdiChildMax] == 0)
				//{
				//	formState[FormStateWindowState] = (int)FormWindowState.Minimized;
				//}
				break;
			case SW_HIDE:
			default:
				break;
		}

		// If we used to be normal and we just became minimized or maximized,
				// stash off our current bounds so we can properly restore.
				//
		if(oldState == FormWindowState::Normal && WindowState() != FormWindowState::Normal)
		{
			if(WindowState() == FormWindowState::Minimized)
			{
				//SuspendLayoutForMinimize();
			}

			m_RestoredWindowBounds.SetSize(GetClientSize());
			m_RestoredWindowBoundsSpecified = BoundsSpecified::Size;
			m_RestoredWindowBounds.SetLocation(GetLocation());
			m_RestoredWindowBoundsSpecified |= BoundsSpecified::Location;

			//stash off restoreBounds As well...
			m_RestoreBounds.SetSize(GetSize());
			m_RestoreBounds.SetLocation(GetLocation());
		}

		// If we just became normal or maximized resume
		if(oldState == FormWindowState::Minimized && WindowState() != FormWindowState::Minimized)
		{
			//ResumeLayoutFromMinimize();
		}

		switch(WindowState())
		{
			case FormWindowState::Normal:
				SetState(STATE_SIZELOCKEDBYOS, false);
				break;
			case FormWindowState::Maximized:
			case FormWindowState::Minimized:
				SetState(STATE_SIZELOCKEDBYOS, true);
				break;
		}

		if(oldState != WindowState())
		{
			//AdjustSystemMenu();
		}
	}
}

//void Window::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
//{
//	graphics->FillRectangle(GetBackgroundColor(), rectangle);
//}

void Window::WmCreate(Message& m)
{
	Control::WmCreate(m);

	STARTUPINFO si = { 0 };
	GetStartupInfo(&si);

	if(IsTopLevel() && (si.dwFlags & STARTF_USESHOWWINDOW) != 0)
	{
		switch(si.wShowWindow)
		{
			case SW_MAXIMIZE: WindowState(FormWindowState::Maximized); break;
			case SW_MINIMIZE: WindowState(FormWindowState::Minimized); break;
		}
	}
}

void Window::WmEraseBackground(Message& m)
{
	UpdateWindowState();
	Control::WmEraseBackground(m);
}

void Window::WmNcButtonDown(Message& m)
{
	if(IsMDIChild())
	{
		Window* wndParent = m_MDIParent;
		if(m_MDIParent->GetActiveMDIChild() == this)
		{
			if(GetActiveControl() != nullptr && !GetActiveControl()->ContainsFocus())
			{
				GetInnerMostActiveControl()->FocusActiveControlInternal();
			}
		}
	}
}

void Window::WmWinPosChanged(Message& m)
{
	UpdateWindowState();
	ContainerControl::WndProc(m);
}

void Window::DefWndProc(Message& m)
{
	if(m_CtlClient != nullptr && m_CtlClient->IsHandleCreated() && m_CtlClient->GetParent() == this)
	{
		m.Result = (void*)DefFrameProc(m.hWnd, m_CtlClient->GetHandle(), m.Msg, m.wParam, m.lParam);
	}
	else if(m_UseMDIChildProc)
	{
		m.Result = (void*)DefMDIChildProc(m.hWnd, m.Msg, m.wParam, m.lParam);
	}
	else
	{
		ContainerControl::DefWndProc(m);
	}
}

void Window::WndProc(Message& m)
{
	switch(m.Msg)
	{
		case WM_NCACTIVATE:
			//if(IsRestrictedWindow)
			//{
			//	BeginInvoke(new MethodInvoker(RestrictedProcessNcActivate));
			//}
			//base.WndProc(ref m);
			break;
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			WmNcButtonDown(m);
			break;
		//case NativeMethods.WM_ACTIVATE:
		//	WmActivate(ref m);
		//	break;
		//case NativeMethods.WM_MDIACTIVATE:
		//	WmMdiActivate(ref m);
		//	break;
		//case NativeMethods.WM_CLOSE:
		//	if(CloseReason == CloseReason.None)
		//	{
		//		CloseReason = CloseReason.TaskManagerClosing;
		//	}
		//	WmClose(ref m);
		//	break;
		//
		//case NativeMethods.WM_QUERYENDSESSION:
		//case NativeMethods.WM_ENDSESSION:
		//	CloseReason = CloseReason.WindowsShutDown;
		//	WmClose(ref m);
		//	break;
		//case NativeMethods.WM_ENTERSIZEMOVE:
		//	WmEnterSizeMove(ref m);
		//	DefWndProc(ref m);
		//	break;
		//case NativeMethods.WM_EXITSIZEMOVE:
		//	WmExitSizeMove(ref m);
		//	DefWndProc(ref m);
		//	break;
		case WM_CREATE:
			WmCreate(m);
			break;
		case WM_ERASEBKGND:
			WmEraseBackground(m);
			break;

		//case NativeMethods.WM_INITMENUPOPUP:
		//	WmInitMenuPopup(ref m);
		//	break;
		//case NativeMethods.WM_UNINITMENUPOPUP:
		//	WmUnInitMenuPopup(ref m);
		//	break;
		//case NativeMethods.WM_MENUCHAR:
		//	WmMenuChar(ref m);
		//	break;
		//case NativeMethods.WM_NCDESTROY:
		//	WmNCDestroy(ref m);
		//	break;
		//case NativeMethods.WM_NCHITTEST:
		//	WmNCHitTest(ref m);
		//	break;
		//case NativeMethods.WM_SHOWWINDOW:
		//	WmShowWindow(ref m);
		//	break;
		//case NativeMethods.WM_SIZE:
		//	WmSize(ref m);
		//	break;
		//case NativeMethods.WM_SYSCOMMAND:
		//	WmSysCommand(ref m);
		//	break;
		//case NativeMethods.WM_GETMINMAXINFO:
		//	WmGetMinMaxInfo(ref m);
		//	break;
		case WM_WINDOWPOSCHANGED:
			WmWinPosChanged(m);
			break;
			//case NativeMethods.WM_WINDOWPOSCHANGING:
			//    WmWindowPosChanging(ref m);
			//    break;
		//case NativeMethods.WM_ENTERMENULOOP:
		//	WmEnterMenuLoop(ref m);
		//	break;
		//case NativeMethods.WM_EXITMENULOOP:
		//	WmExitMenuLoop(ref m);
		//	break;
		//case NativeMethods.WM_CAPTURECHANGED:
		//	base.WndProc(ref m);
		//	// This is a work-around for the Win32 scroll bar; it
		//	// doesn't release it's capture in response to a CAPTURECHANGED
		//	// message, so we force capture away if no button is down.
		//	//
		//	if(CaptureInternal && MouseButtons == (MouseButtons)0)
		//	{
		//		CaptureInternal = false;
		//	}
		//	break;
		//case NativeMethods.WM_GETDPISCALEDSIZE:
		//{
		//	Debug.Assert(NativeMethods.Util.SignedLOWORD(m.WParam) == NativeMethods.Util.SignedHIWORD(m.WParam), "Non-square pixels!");
		//	if(DpiHelper.EnableDpiChangedMessageHandling)
		//	{
		//		WmGetDpiScaledSize(ref m);
		//	}
		//	else
		//	{
		//		m.Result = IntPtr.Zero;
		//	}
		//}
		//break;
		//case NativeMethods.WM_DPICHANGED:
		//	if(DpiHelper.EnableDpiChangedMessageHandling)
		//	{
		//		WmDpiChanged(ref m);
		//		// If the application processes this message, return zero.
		//		m.Result = IntPtr.Zero;
		//	}
		//	else
		//	{
		//		m.Result = (IntPtr)1;
		//	}
		//	break;
		default:
			ContainerControl::WndProc(m);
			break;
	}
}

void Window::SetBoundsCore(int x, int y, int width, int height, BoundsSpecified specified)
{
	if(WindowState() != FormWindowState::Normal)
	{
		if(x != -1 || y != -1)
		{
			m_RestoredWindowBoundsSpecified |= (specified & (BoundsSpecified::X | BoundsSpecified::Y));
		}

		m_RestoredWindowBoundsSpecified |= (specified & (BoundsSpecified::Width | BoundsSpecified::Height));

		if((specified & BoundsSpecified::X) != 0)
			m_RestoredWindowBounds.X = x;
		if((specified & BoundsSpecified::Y) != 0)
			m_RestoredWindowBounds.Y = y;
		if((specified & BoundsSpecified::Width) != 0)
		{
			m_RestoredWindowBounds.Width = width;
			//formStateEx[FormStateExWindowBoundsWidthIsClientSize] = 0;
		}
		if((specified & BoundsSpecified::Height) != 0)
		{
			m_RestoredWindowBounds.Height = height;
			//formStateEx[FormStateExWindowBoundsHeightIsClientSize] = 0;
		}
	}

	//Update RestoreBounds
	if((specified & BoundsSpecified::X) != 0)
		m_RestoreBounds.X = x;
	if((specified & BoundsSpecified::Y) != 0)
		m_RestoreBounds.Y = y;
	if((specified & BoundsSpecified::Width) != 0 || m_RestoreBounds.Width == -1)
		m_RestoreBounds.Width = width;
	if((specified & BoundsSpecified::Height) != 0 || m_RestoreBounds.Height == -1)
		m_RestoreBounds.Height = height;

	// Enforce maximum size...
	if(WindowState() == FormWindowState::Normal && (m_Height != height || m_Width != width))
	{

		Size max = SystemInformation::MaxWindowTrackSize();
		if(height > max.Height)
		{
			height = max.Height;
		}
		if(width > max.Width)
		{
			width = max.Width;
		}
	}

	// Only enforce the minimum size if the form has a border and is a top
	// level form.
	FormBorderStyle borderStyle = GetFormBorderStyle();
	if(borderStyle != FormBorderStyle::None
	   && borderStyle != FormBorderStyle::FixedToolWindow
	   && borderStyle != FormBorderStyle::SizableToolWindow
	   && Parent == nullptr)
	{

		Size min = SystemInformation::MinWindowTrackSize();
		if(height < min.Height)
		{
			height = min.Height;
		}
		if(width < min.Width)
		{
			width = min.Width;
		}
	}

	//if(IsRestrictedWindow)
	//{
		// Check to ensure that the title bar, and all corners of the window, are visible on a monitor
		Drawing::Rectangle adjustedBounds = ApplyBoundsConstraints(x, y, width, height);
		if(adjustedBounds != Drawing::Rectangle(x, y, width, height))
		{

			// SECREVIEW VSWhidbey 430541 - the base will call ApplyBoundsConstraints again, so
			// we're still OK (even though we have security code in a virtual function).
			//
			// Keeping this here for compat reasons - we want to  manually call base here with BoundsSpecified.All 
			// so we ---- over the specified bounds.
			Control::SetBoundsCore(adjustedBounds.X, adjustedBounds.Y, adjustedBounds.Width, adjustedBounds.Height, BoundsSpecified::All);
			return;
		}
	//}

	Control::SetBoundsCore(x, y, width, height, specified);
}

bool Window::IsCursorEnabled() const noexcept
{
	return m_IsCursorEnabled;
}

Keyboard& Window::GetKeyboard() const
{
	if(!m_Keyboard)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Keyboard;
}

Mouse& Window::GetMouse() const
{
	if(!m_Mouse)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Mouse;
}

FormWindowState Window::WindowState()
{
	return m_FormWindowState;
}

void Window::WindowState(FormWindowState value)
{
	if(IsTopLevel())
	{
		if(value != FormWindowState::Normal) return;
	}

	switch(value)
	{
		case FormWindowState::Normal: SetState(STATE_SIZELOCKEDBYOS, false); break;
		case FormWindowState::Maximized:
		case FormWindowState::Minimized:
		{
			SetState(STATE_SIZELOCKEDBYOS, true);
			break;
		}
	}

	if(IsHandleCreated() && IsVisible())
	{
		IntPtr hWnd = GetHandle();
		switch(value)
		{
			case FormWindowState::Normal:
				ShowWindow(hWnd, SW_NORMAL);
				break;
			case FormWindowState::Maximized:
				ShowWindow(hWnd, SW_MAXIMIZE);
				break;
			case FormWindowState::Minimized:
				ShowWindow(hWnd, SW_MINIMIZE);
				break;
		}
	}

	m_FormWindowState = value;
}

FormBorderStyle Window::GetFormBorderStyle()
{
	return m_FormBorderStyle;
}

void Window::SetFormBorderStyle(FormBorderStyle value)
{
	//TODO: CHECK FOR RESTRICTED WINDOW
	//switch(value)
	//{
	//
	//}

	m_FormBorderStyle = value;

	//if(formState[FormStateSetClientSize] == 1 && !IsHandleCreated)
	//{
	//	ClientSize = ClientSize;
	//}

	Drawing::Rectangle preClientUpdateRestoredWindowBounds = m_RestoredWindowBounds;
	BoundsSpecified preClientUpdateRestoredWindowBoundsSpecified = m_RestoredWindowBoundsSpecified;

	//UpdateFormStyles();

	//if(formState[FormStateIconSet] == 0 && !IsRestrictedWindow)
	//{
	//	UpdateWindowIcon(false);
	//}

	// VSWhidbey 95327: Now restore the values cached above.
	if(WindowState() != FormWindowState::Normal)
	{
		m_RestoredWindowBounds = preClientUpdateRestoredWindowBounds;
		m_RestoredWindowBoundsSpecified = preClientUpdateRestoredWindowBoundsSpecified;
		//formStateEx[FormStateExWindowBoundsWidthIsClientSize] = preWindowBoundsWidthIsClientSize;
		//formStateEx[FormStateExWindowBoundsHeightIsClientSize] = preWindowBoundsHeightIsClientSize;
	}
}