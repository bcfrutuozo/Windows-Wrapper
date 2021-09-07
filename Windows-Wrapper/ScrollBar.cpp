#include "ScrollBar.h"
#include "ScrollableControl.h"

int ScrollBar::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;
}

void ScrollBar::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(ps.hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
}

ScrollBar::ScrollBar(ScrollableControl* parent, int width, int height, int x, int y)
	:
	Control(parent, "", width, height, x, y),
	Owner(parent),
	IsScrollVisible(false),
	Scrolling(0),
	MaximumValue(100)
{
	//Initialize();
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
		Location.X,										// X position
		Location.Y,										// Y position
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

void ScrollBar::Show() noexcept
{
	Control::Show();

	if (IsShown())
	{
		IsScrollVisible = true;
	}
}

void ScrollBar::Hide() noexcept
{
	Control::Hide();

	if (IsShown())
	{
		IsScrollVisible = false;
	}
}

size_t ScrollBar::GetScrolling() const noexcept
{
	return Scrolling;
}

void ScrollBar::SetMaximumValue(size_t maxValue) noexcept
{
	MaximumValue = maxValue;
}