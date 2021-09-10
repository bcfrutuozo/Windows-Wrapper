#include "HorizontalScrollBar.h"
#include "ScrollableControl.h"

void HorizontalScrollBar::OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept
{
	int nPos;
	int nOldPos;
	SCROLLINFO si;

	// Get current scrollbar state:
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	if (GetScrollInfo(hwnd, SB_HORZ, &si) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	nOldPos = si.nPos;

	// Compute new nPos.
	// Note we do not care where nPos falls between nMin and nMax. See below.
	switch (code) {
	case SB_LEFT:            nPos = si.nMin; break;
	case SB_RIGHT:         nPos = si.nMax; break;
	case SB_LINELEFT:         nPos = si.nPos - 1; break;
	case SB_LINERIGHT:       nPos = si.nPos + 1; break;
		//case SB_PAGELEFT:         nPos = si.nPos - CustomLogicalPage(si.nPage); break;
		//case SB_PAGERIGHT:       nPos = si.nPos + CustomLogicalPage(si.nPage); break;
	case SB_THUMBTRACK:     nPos = pos; break;
	default:
	case SB_THUMBPOSITION:  nPos = si.nPos; break;
	}

	// Update the scrollbar state (nPos) and repaint it. The function ensures
	// the nPos does not fall out of the allowed range between nMin and nMax
	// hence we ask for the corrected nPos again.
	if (nPos < 0)
	{
		return;
	}

	if (nPos > si.nMax)
	{
		return;
	}

	SetScrollPos(hwnd, SB_HORZ, nPos, true);
	Scrolling = GetScrollPos(hwnd, SB_HORZ);

	// Refresh the control (repaint it to reflect the new nPos). Note we
	// here multiply with some unspecified scrolling unit which specifies
	// amount of pixels corresponding to the 1 scrolling unit.
	// We will discuss ScrollWindowEx() more later in the article.
	nOldPos > nPos ? Owner->DecrementHorizontalScroll() : Owner->IncrementHorizontalScroll();
	WinAPI::OnHorizontalScrolling_Impl(hwnd, hwndCtl, code, pos);
}

void HorizontalScrollBar::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept
{
	if (ShowScrollBar(hwnd, SB_HORZ, IsScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	if (IsScrollVisible)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = cx;
		SetScrollInfo(hwnd, SB_HORZ, &si, true);
		si.nMin = 0;
		si.nMax = MaximumValue - 1;
		si.nPage = cy;
		si.nPos = 0;
		si.nTrackPos = 0;
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