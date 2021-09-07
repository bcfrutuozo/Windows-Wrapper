#include "ComboBox.h"

void ComboBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	RECT rect;

	InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));
	rect.left = rect.right - GetSystemMetrics(SM_CXVSCROLL);
	
	WinAPI::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void ComboBox::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	RECT rect;
	RECT rect2;
	BeginPaint(hwnd, &ps);

	//
	//	Mask off the borders and draw ComboBox normally
	//
	GetClientRect(hwnd, &rect);

	InflateRect(&rect, -GetSystemMetrics(SM_CXEDGE), -GetSystemMetrics(SM_CYEDGE));

	rect.right -= GetSystemMetrics(SM_CXVSCROLL);

	IntersectClipRect(ps.hdc, rect.left, rect.top, rect.right, rect.bottom);

	// Draw the ComboBox
	//HandleMessageForwarder(hwnd, WM_PAINT, (WPARAM)ps.hdc, 0);
	//CallWindowProc(OldComboProc, hwnd, msg, (WPARAM)ps.hdc, 0);

	//
	//	Now mask off inside and draw the borders
	//
	SelectClipRgn(ps.hdc, NULL);
	rect.right += GetSystemMetrics(SM_CXVSCROLL);

	ExcludeClipRect(ps.hdc, rect.left, rect.top, rect.right, rect.bottom);

	// draw borders
	GetClientRect(hwnd, &rect2);
	FillRect(ps.hdc, &rect2, GetSysColorBrush(COLOR_3DSHADOW));

	// now draw the button
	SelectClipRgn(ps.hdc, NULL);
	rect.left = rect.right - GetSystemMetrics(SM_CXVSCROLL);

	if (IsClicking())
	{
		DrawFrameControl(ps.hdc, &rect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT | DFCS_PUSHED);
		//FillRect(hdc, &rect, GetSysColorBrush(COLOR_3DDKSHADOW));
		
		// SHOW LISTBOX
	}
	else
		DrawFrameControl(ps.hdc, &rect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT);
	//FillRect(hdc, &rect, GetSysColorBrush(COLOR_3DFACE));

	EndPaint(hwnd, &ps);
}

ComboBox::ComboBox(Control* parent, const std::string& name, int width, int x, int y)
	:
	ListControl(parent, name, width, 0, x, y),	// Default control size without font is 9
	m_FlatStyle(FlatStyle::Standard)
{

}

ComboBox::~ComboBox()
{

}

void ComboBox::IncrementHorizontalScroll() noexcept
{
}

void ComboBox::DecrementHorizontalScroll() noexcept
{
}

void ComboBox::IncrementVerticalScroll() noexcept
{
}

void ComboBox::DecrementVerticalScroll() noexcept
{
}
