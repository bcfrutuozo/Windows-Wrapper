#include "Window.h"
#include "OnClosedEventHandler.h"
#include "OnClosingEventHandler.h"
#include "Application.h"

Window::Window(const std::string& name, int width, int height)
	:
	Control(nullptr, name, width, height, 0, 0),
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
	m_FormStateShowWindowOnCreate(false)
{

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
		if(IsShown() && (m_FormWindowState == FormWindowState::Maximized || m_FormWindowState == FormWindowState::Normal))
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
	if(IsShown())
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

void Window::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{
	graphics->FillRectangle(GetBackgroundColor(), rectangle);
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