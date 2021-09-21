#include "VerticalScrollBar.h"
#include "ScrollableControl.h"
#include "ControlException.h"

void VerticalScrollBar::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept
{
	if (ShowScrollBar(hwnd, SB_VERT, true) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	if (IsShown())
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = MaximumValue - 1;
		si.nPage = cy;
		SetScrollInfo(hwnd, SB_VERT, &si, true);
	}

	WinAPI::OnSize_Impl(hwnd, state, cx, cy);
}

void VerticalScrollBar::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept
{
	int nPos;
	int nOldPos;
	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	GetScrollInfo(hwnd, SB_VERT, &si);

	nOldPos = si.nPos;

	switch (code) {
	case SB_TOP:            nPos = si.nMin; break;
	case SB_BOTTOM:         nPos = si.nMax; break;
	case SB_LINEUP:         nPos = si.nPos - 1; break;
	case SB_LINEDOWN:       nPos = si.nPos + 1; break;
	case SB_PAGEUP:         nPos = si.nPos - Owner->GetVerticalPage(); break;
	case SB_PAGEDOWN:       nPos = si.nPos + Owner->GetVerticalPage(); break;
	case SB_THUMBTRACK:     nPos = pos; break;
	default:
	case SB_THUMBPOSITION:  nPos = si.nPos; break;
	}

	if (nPos < 0)
	{
		nPos = 0;
	}
	else if (nPos > si.nMax)
	{
		nPos = si.nMax;
	}

	SetScrollPos(hwnd, SB_VERT, nPos, true);
	Scrolling = GetScrollPos(hwnd, SB_VERT);

	InvalidateRect(static_cast<HWND>(Owner->Handle.ToPointer()), Owner->GetDrawableArea(), false);	
	WinAPI::OnVerticalScrolling_Impl(hwnd, hwndCtl, code, pos);
}

VerticalScrollBar::VerticalScrollBar(ScrollableControl* parent, int width, int height, int x, int y)
	:
	ScrollBar(parent, width, height, x, y)
{

}

VerticalScrollBar::~VerticalScrollBar()
{

}