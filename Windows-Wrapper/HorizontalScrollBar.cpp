#include "HorizontalScrollBar.h"
#include "ScrollableControl.h"

void HorizontalScrollBar::OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos)
{
	if (GetFocus() != static_cast<HWND>(Parent->Handle.ToPointer())) SetFocus(static_cast<HWND>(Parent->Handle.ToPointer()));

	int nPos;
	int nOldPos;
	SCROLLINFO si = { 0 };

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	if (GetScrollInfo(hwnd, SB_HORZ, &si) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	nOldPos = si.nPos;

	switch (code) {
	case SB_LEFT:            nPos = si.nMin; break;
	case SB_RIGHT:         nPos = si.nMax; break;
	case SB_LINELEFT:         nPos = si.nPos - 1; break;
	case SB_LINERIGHT:       nPos = si.nPos + 1; break;
	case SB_PAGELEFT:         nPos = si.nPos - Owner->GetHorizontalPage(); break;
	case SB_PAGERIGHT:       nPos = si.nPos + Owner->GetHorizontalPage(); break;
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

	SetScrollPos(hwnd, SB_HORZ, nPos, true);
	Scrolling = GetScrollPos(hwnd, SB_HORZ);

	InvalidateRect(static_cast<HWND>(Owner->Handle.ToPointer()), Owner->GetDrawableArea(), false);
	WinAPI::OnHorizontalScrolling_Impl(hwnd, hwndCtl, code, pos);
}

void HorizontalScrollBar::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy)
{
	if (ShowScrollBar(hwnd, SB_HORZ, true) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	if (IsShown())
	{
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = MaximumValue - 1;
		si.nPage = cx;
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
	}

	WinAPI::OnSize_Impl(hwnd, state, cx, cy);
}

HorizontalScrollBar::HorizontalScrollBar(ScrollableControl* parent, int width, int height, int x, int y)
	:
	ScrollBar(parent, width, height, x, y)
{
}

HorizontalScrollBar::~HorizontalScrollBar()
{

}