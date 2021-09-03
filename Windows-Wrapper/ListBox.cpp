#include "ListBox.h"

void ListBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	switch (vk)
	{
	case VK_DOWN:
	{
		if (m_SelectedIndex < GetDataSource()->GetCount())
		{
			++m_SelectedIndex;
		}

		break;
	}
	case VK_UP:
	{
		if (m_SelectedIndex > 0)
		{
			--m_SelectedIndex;
		}

		break;
	}
	}

	Update();
	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void ListBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	size_t i = 0;
	for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it, ++i)
	{
		if (y >= it->top && y <= it->bottom && x >= it->left && x <= it->right)
		{
			SetSelectedIndex(i);
			break;
		}
	}

	Control::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void ListBox::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdcMem, &ps.rcPaint, bgColor);
	SelectObject(hdcMem, bgColor);
	DeleteObject(bgColor);

	HFONT hFont = CreateFont(
		m_Font.GetSizeInPixels(),
		0,
		0,
		0,
		m_Font.IsBold() ? FW_BOLD : FW_NORMAL,
		m_Font.IsItalic(),
		m_Font.IsUnderline(),
		m_Font.IsStrikeOut(),
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_Font.GetName().c_str());

	SelectObject(hdcMem, hFont);

	// Example test draw with the desired font to calculate each ListBox item size

	const char* verifier = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	GetTextExtentPoint32(hdcMem, verifier, 27, &m_SingleSize);

	const auto& dataSource = GetDataSource();
	m_RowPosition.clear();
	if (m_RowPosition.size() < dataSource->GetCount())
	{
		m_RowPosition.resize(dataSource->GetCount());
	}

	// Drawable block inside ListBox
	RECT r(m_Margin.Left, m_Margin.Top, m_Size.Width - m_Margin.Right, m_Size.Height - m_Margin.Bottom);
	if ((static_cast<unsigned long long>(r.bottom) - r.top) < (m_SingleSize.cy * dataSource->GetCount()))
	{
		m_IsVerticalScrollVisible = true;
	}
	else
	{
		m_IsVerticalScrollVisible = false;
	}

	SCROLLINFO si;
	// Get current scrollbar state:
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	GetScrollInfo(hwnd, SB_VERT, &si);

	size_t i = 0;
	for (auto it = dataSource->begin(); it != dataSource->end(); ++it, ++i)
	{
		if (m_IsMultiColumn)
		{
			int columnNumber = (m_Size.Width - m_Margin.Right) / m_SingleSize.cx;
		}
		else
		{
			RECT cr;
			CopyRect(&cr, &r);
			cr.top = (m_SingleSize.cy * i) - (si.nPos * m_SingleSize.cy);
			cr.bottom = (cr.top + m_SingleSize.cy);
			m_RowPosition[i] = cr;

			if (cr.bottom < m_Size.Height)
			{
				if (m_SelectedIndex == i)
				{
					SetBkColor(hdcMem, RGB(0, 120, 215));
					SetTextColor(hdcMem, RGB(255, 255, 255));
				}
				else
				{
					SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
				}

				DrawText(hdcMem, (*dataSource)[i]->Value.c_str(), -1, &cr, DT_LEFT | DT_VCENTER);
				DrawText(hdcMem, Text.c_str(), static_cast<int>((*dataSource)[i]->Value.length()), &cr, DT_LEFT | DT_VCENTER | DT_CALCRECT);
			}
		}
	}

	auto totalSizeVertical = m_RowPosition.back().bottom - m_RowPosition.front().top;
	auto totalInScreen = m_Size.Height / m_SingleSize.cy;
	auto remainingSteps = dataSource->GetCount() - totalInScreen;
	m_VerticalScrollPaging = ((static_cast<float>(m_Size.Height) / static_cast<float>(totalSizeVertical))) * 100.0f;

	HandleMessageForwarder(hwnd, WM_SIZE, MAKEWPARAM(0, 0), MAKELPARAM(m_RowPosition.back().right - m_RowPosition.back().left, totalInScreen + 1));

	if (ShowScrollBar(hwnd, SB_VERT, m_IsVerticalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(ps.hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hFont);
	DeleteObject(hFont);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);

	EndPaint(hwnd, &ps);
}

void ListBox::OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept
{
	if (m_IsVerticalScrollVisible)
	{
		SCROLLINFO si;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;

		si.nPage = cx;
		SetScrollInfo(hwnd, SB_HORZ, &si, FALSE);

		// FIX: Make sure uHeight has the right value:
		/*{
			RECT rc;
			GetClientRect(hwnd, &rc);
			cy = rc.bottom - rc.top;
		}*/
		auto maxSize = GetDataSource()->GetCount();
		si.nMin = 0;
		si.nMax = maxSize;
		si.nPage = cy;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
	}
}

void ListBox::OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos) noexcept
{
	std::ostringstream oss;

	int nPos;
	int nOldPos;
	SCROLLINFO si;

	// Get current scrollbar state:
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;
	GetScrollInfo(hwnd, SB_VERT, &si);

	nOldPos = si.nPos;

	// Compute new nPos.
	// Note we do not care where nPos falls between nMin and nMax. See below.
	switch (code) {
	case SB_TOP:            nPos = si.nMin; break;
	case SB_BOTTOM:         nPos = si.nMax; break;
	case SB_LINEUP:         nPos = si.nPos - 1; break;
	case SB_LINEDOWN:       nPos = si.nPos + 1; break;
		//case SB_PAGEUP:         nPos = si.nPos - CustomLogicalPage(si.nPage); break;
		//case SB_PAGEDOWN:       nPos = si.nPos + CustomLogicalPage(si.nPage); break;
	case SB_THUMBTRACK:     nPos = si.nTrackPos; break;
	default:
	case SB_THUMBPOSITION:  nPos = si.nPos; break;
	}

	oss << "cbSize: " << si.cbSize << std::endl
		<< "fMask: " << si.fMask << std::endl
		<< "nMin: " << si.nMin << std::endl
		<< "nMax: " << si.nMax << std::endl
		<< "nPage: " << si.nPage << std::endl
		<< "nPos: " << si.nPos << std::endl
		<< "nTrackPos: " << si.nTrackPos << std::endl << std::endl;

	printf_s(oss.str().c_str());

	// Update the scrollbar state (nPos) and repaint it. The function ensures
	// the nPos does not fall out of the allowed range between nMin and nMax
	// hence we ask for the corrected nPos again.
	if (nPos < 0)
	{
		return;
	}

	auto maxValue = GetDataSource()->GetCount();
	if (nPos > maxValue)
	{
		return;
	}

	SetScrollPos(hwnd, SB_VERT, nPos, TRUE);
	m_VerticalScrolling = GetScrollPos(hwnd, SB_VERT);

	// Refresh the control (repaint it to reflect the new nPos). Note we
	// here multiply with some unspecified scrolling unit which specifies
	// amount of pixels corresponding to the 1 scrolling unit.
	// We will discuss ScrollWindowEx() more later in the article.
	auto up = nOldPos > nPos;
	
	for (auto it = m_RowPosition.begin(); it != m_RowPosition.end(); ++it)
	{
		if (up)
		{
			it->bottom -= m_SingleSize.cy;
			it->top -= m_SingleSize.cy;
		}
		else
		{
			it->bottom += m_SingleSize.cy;
			it->top += m_SingleSize.cy;
		}
	}

	Update();

	//ScrollWindowEx(hwnd, 0, inc, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE);
}

ListBox::ListBox(Control* parent, int width, int height, int x, int y)
	:
	ListControl(parent, "", width, height, x, y),
	m_IsMultiColumn(false),
	m_IsHorizontalScrollVisible(false),
	m_IsVerticalScrollVisible(false),
	m_IsScrollAlwaysVisible(false),
	m_SelectionMode(SelectionMode::Single),
	m_DockStyle(DockStyle::None),
	m_BorderStyle(BorderStyle::Fixed3D),
	m_SingleSize({ 0 })
{
	if (ShowScrollBar(static_cast<HWND>(Handle.ToPointer()), SB_HORZ, m_IsHorizontalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}

	if (ShowScrollBar(static_cast<HWND>(Handle.ToPointer()), SB_VERT, m_IsVerticalScrollVisible) == 0)
	{
		throw CTL_LAST_EXCEPT();
	}
}

ListBox::~ListBox()
{

}