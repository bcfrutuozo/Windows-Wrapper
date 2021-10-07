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
	m_IsMenuStripEnabled(false)
{
	Initialize();
}

void Window::OnClosingSet(const std::function<void(Object*, OnClosingEventArgs*)>& callback) noexcept
{
	Events.Register(new OnClosingEventHandler("OnClosing", callback));
}

void Window::OnShownSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept
{
	Events.Register(new EventHandler("OnShown", callback));
}

void Window::OnClosedSet(const std::function<void(Object*, OnClosedEventArgs*)>& callback) noexcept
{
	Events.Register(new OnClosedEventHandler("OnClosing", callback));
}

void Window::Initialize()
{
	// Calculate window size based on desired client region
	RECT r = { 0 };
	r.left = 100;
	r.right = m_Size.Width + r.left;
	r.top = 100;
	r.bottom = m_Size.Height + r.top;
	if (AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE, FALSE) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),																								// Class name
		Text.c_str(),																										// Window title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_VISIBLE,		// Style values
		CW_USEDEFAULT,																										// X position
		CW_USEDEFAULT,																										// Y position
		(r.right - r.left),																									// Width
		(r.bottom - r.top),																									// Height
		nullptr,																											// Parent handle
		nullptr,																											// Menu handle
		WindowClass::GetInstance(),																							// Module instance handle
		this																												// Pointer to the window instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// TODO: THIS IS WHERE THE GRAPHICS DEVICE, CONTEXT, RENDER TARGET, DEPTHSTENCIL (MAYBE) WILL BE INSTANTIATED.
	// YES: THIS IS GONNA TAKE SOME TIME DO DESIGN
	// AND YES AS WELL: WILL BE PROBABLY POORLY DONE :( 

	// Register mouse raw input device
	RAWINPUTDEVICE rid = { 0 };
	rid.usUsagePage = 0x01; // Mouse page
	rid.usUsage = 0x02; // Mouse page
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false)
	{
		throw CTL_LAST_EXCEPT();
	}
	// Still don't know how I'll implement child windows. So this is just to not let an empty ugly function
	//SetParent(static_cast<HWND>(Handle.ToPointer()), static_cast<HWND>(Parent->Handle.ToPointer()));

	// Force window redraw to set Background color
	m_BackgroundColor = Color::WindowBackground();

	// Add window the the application windows container
	Application::AddWindow(this);
	Update();
}

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

Button* Window::AddButton(const std::string& name, int width, int height, int x, int y) noexcept
{
	return Create<Button>(this, name, width, height, x, y);
}

TextBox* Window::AddTextBox(const std::string& name, int width, int x, int y) noexcept
{
	return Create<TextBox>(this, name, width, x, y);
}

ListBox* Window::AddListBox(int width, int height, int x, int y) noexcept
{
	return Create<ListBox>(this, width, height, x, y);
}

ComboBox* Window::AddComboBox(const std::string& name, int width, int x, int y) noexcept
{
	return Create<ComboBox>(this, name, width, x, y);
}

ProgressBar* Window::AddProgressBar(int width, int height, int x, int y) noexcept
{
	return Create<ProgressBar>(this, width, height, x, y);
}

ProgressBar* Window::AddProgressBar(const std::string& text, int width, int height, int x, int y) noexcept
{
	return Create<ProgressBar>(this, text, width, height, x, y);
}

ToolStrip* Window::AddToolStrip() noexcept
{
	ToolStrip* t = Create<ToolStrip>(this);
	m_MinSize += t->GetSize().Height; // Invalidate the window header size
	return t;
}

Label* Window::AddLabel(const std::string& text, int x, int y) noexcept
{
	return Create<Label>(this, text, x, y);
}

void Window::SetText(const std::string& title)
{
	if (IsShown())
	{
		if (SetWindowText(static_cast<HWND>(Handle.ToPointer()), title.c_str()) == 0)
		{
			throw CTL_LAST_EXCEPT();
		}

		Text = title;

		if (RedrawWindow(static_cast<HWND>(Handle.ToPointer()), NULL, NULL, RDW_INVALIDATE | RDW_ERASE) == 0)
		{
			throw CTL_LAST_EXCEPT();
		}
	}
}

void Window::EnableCursor() noexcept
{
	if (m_Mouse != nullptr)
	{
		m_IsCursorEnabled = true;
		ShowCursor();
		FreeCursor();
	}
}

void Window::DisableCursor() noexcept
{
	if (m_Mouse != nullptr)
	{
		m_IsCursorEnabled = false;
		HideCursor();
		EncloseCursor();
	}
}

void Window::EncloseCursor() const noexcept
{
	RECT r;
	GetClientRect(static_cast<HWND>(Handle.ToPointer()), &r);
	MapWindowPoints(static_cast<HWND>(Handle.ToPointer()), nullptr, reinterpret_cast<POINT*>(&r), 2);
	ClipCursor(&r);
}

void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}

void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{
	auto bgColor = GetBackgroundColor();
	graphics->CreateSolidBrush(bgColor.ToString(), bgColor);
	graphics->FillRectangle(rectangle, bgColor.ToString());
}

bool Window::IsCursorEnabled() const noexcept
{
	return m_IsCursorEnabled;
}

Keyboard& Window::GetKeyboard() const
{
	if (!m_Keyboard)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Keyboard;
}

Mouse& Window::GetMouse() const
{
	if (!m_Mouse)
	{
		//TODO: DEVICE EXCEPTION
		throw;
	}

	return *m_Mouse;
}