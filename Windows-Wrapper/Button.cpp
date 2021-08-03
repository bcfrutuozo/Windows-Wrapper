#include "Button.h"
#include "Window.h"

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y)
{
	Initialize();
}

Button::~Button()
{

}

void Button::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		"Button",																							// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | SS_CENTER | BS_PUSHBUTTON | WS_CLIPSIBLINGS,					// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		Size.Width,																							// Width
		Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                												// Menu handle
		Window::WndClass::GetInstance(),																	// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
}

void Button::ChangeColor(Color color) noexcept
{

}

void Button::Disable() noexcept
{

}

void Button::Enable() noexcept
{

}

void Button::Hide()
{
	if (IsShown())
	{
		IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void Button::Show()
{
	if (!IsShown())
	{
		IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}