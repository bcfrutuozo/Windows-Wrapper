#include "ScrollBar.h"
#include "ScrollableControl.h"

int ScrollBar::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	/*RECT rc = { 0 };
	GetClientRect(hwnd, &rc);
	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdc, &rc, bgColor);
	SelectObject(hdc, bgColor);*/
	return 1;
}

void ScrollBar::OnPaint_Impl(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

ScrollBar::ScrollBar(ScrollableControl* parent, int width, int height, int x, int y)
	:
	Control(parent, "", width, height, x, y),
	Owner(parent),
	Scrolling(0),
	MaximumValue(100)
{
	// ScrollBar Initialize() is called from the control which handles it
}

ScrollBar::~ScrollBar()
{
}

void ScrollBar::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_CLIPSIBLINGS,						// Style values
		m_Location.X,										// X position
		m_Location.Y,										// Y position
		m_Size.Width,									// Width
		m_Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		nullptr,										// Menu handle
		WindowClass::GetInstance(),						// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
}

int ScrollBar::GetScrolling() const noexcept
{
	return Scrolling;
}

void ScrollBar::SetMaximumValue(int maxValue) noexcept
{
	MaximumValue = maxValue;
}