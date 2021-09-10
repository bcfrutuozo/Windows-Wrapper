#include "VerticalScrollBar.h"
#include "ScrollableControl.h"

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
		si.nPage = cx;
		SetScrollInfo(hwnd, SB_HORZ, &si, false);
		si.nMin = 0;
		si.nMax = MaximumValue - 1;
		si.nPage = cy;
		si.nPos = 0;
		si.nTrackPos = 0;
		SetScrollInfo(hwnd, SB_VERT, &si, true);
	}

	WinAPI::OnSize_Impl(hwnd, state, cx, cy);
}

void VerticalScrollBar::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept
{
	int nPos;
	int nOldPos;
	SCROLLINFO si;

	// Get current scrollbar state:
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	GetScrollInfo(hwnd, SB_VERT, &si);

	nOldPos = si.nPos;

	/*std::ostringstream oss;
	oss << "Code: " << code << " | Pos: " << pos << std::endl
		<< "cbSize: " << si.cbSize << std::endl
		<< "fMask: " << si.fMask << std::endl
		<< "nMax: " << si.nMax << std::endl
		<< "nMin: " << si.nMin << std::endl
		<< "nPage: " << si.nPage << std::endl
		<< "nPos: " << si.nPos << std::endl
		<< "nTrackPos: " << si.nTrackPos << std::endl << std::endl;

	printf_s(oss.str().c_str());*/

	// Compute new nPos.
	// Note we do not care where nPos falls between nMin and nMax. See below.
	switch (code) {
	case SB_TOP:            nPos = si.nMin; break;
	case SB_BOTTOM:         nPos = si.nMax; break;
	case SB_LINEUP:         nPos = si.nPos - 1; break;
	case SB_LINEDOWN:       nPos = si.nPos + 1; break;
		//case SB_PAGEUP:         nPos = si.nPos - CustomLogicalPage(si.nPage); break;
		//case SB_PAGEDOWN:       nPos = si.nPos + CustomLogicalPage(si.nPage); break;
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

	/*if (code != SB_THUMBTRACK && nPos + si.nPage > MaximumValue)
	{
		return;
	}

	if (code == SB_THUMBTRACK && nPos + si.nPage > MaximumValue)
	{
		nPos = MaximumValue - si.nPage;
	}*/

	SetScrollPos(hwnd, SB_VERT, nPos, true);
	Scrolling = GetScrollPos(hwnd, SB_VERT);

	// Refresh the control (repaint it to reflect the new nPos). Note we
	// here multiply with some unspecified scrolling unit which specifies
	// amount of pixels corresponding to the 1 scrolling unit.
	// We will discuss ScrollWindowEx() more later in the article.
	nOldPos > nPos ? Owner->DecrementVerticalScroll() : Owner->IncrementVerticalScroll();
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