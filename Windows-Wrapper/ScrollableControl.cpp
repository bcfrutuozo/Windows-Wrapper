#include "ScrollableControl.h"
#include "ScrollBar.h"
#include "Exceptions.h"
#include "ScrollEventType.h"
#include "ScrollEventArgs.h"

Drawing::Rectangle ScrollableControl::GetDisplayRectInternal() noexcept
{
	if(m_DisplayRectangle.IsEmpty()) m_DisplayRectangle = GetClientRectangle();

	if(!HasAutoScroll() && HorizontalScroll.IsVisible()) m_DisplayRectangle = Drawing::Rectangle(m_DisplayRectangle.X, m_DisplayRectangle.Y, HorizontalScroll.GetMaximum(), m_DisplayRectangle.Height);
	if(!HasAutoScroll() && VerticalScroll.IsVisible()) m_DisplayRectangle = Drawing::Rectangle(m_DisplayRectangle.X, m_DisplayRectangle.Y, m_DisplayRectangle.Width, VerticalScroll.GetMaximum());

	return m_DisplayRectangle;
}

int ScrollableControl::ScrollThumbPosition(int fnBar)
{
	SCROLLINFO si;
	si.fMask = SIF_TRACKPOS;
	GetScrollInfo(GetHandle(), fnBar, &si);
	return si.nTrackPos;
}

void ScrollableControl::UpdateFullDrag()
{
	SetScrollState(ScrollStateFullDrag, SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, nullptr, 0));
}

void ScrollableControl::SyncScrollbars(bool hasAutoScroll)
{
	Drawing::Rectangle displayRect = m_DisplayRectangle;

	if(HasAutoScroll())
	{
		if(!IsHandleCreated()) return;

		if(GetHScroll())
		{
			if(!HorizontalScroll.m_MaximumSetExternally)
			{
				HorizontalScroll.SetMaximum(m_DisplayRectangle.Width - 1);
			}
			if(!HorizontalScroll.m_LargeChangeSetExternally)
			{
				HorizontalScroll.SetLargeChange(GetClientRectangle().Width);
			}
			if(!HorizontalScroll.m_SmallChangeSetExternally)
			{
				HorizontalScroll.SetSmallChange(5);
			}
			if(m_ResetRTLHScrollValue && !IsMirrored())
			{
				m_ResetRTLHScrollValue = false;
				OnSetScrollPosition(this, new EventArgs());
			}
			else if(-m_DisplayRectangle.X >= HorizontalScroll.GetMinimum() && -m_DisplayRectangle.X < HorizontalScroll.GetMaximum())
			{
				HorizontalScroll.SetValue(-m_DisplayRectangle.X);
			}

			HorizontalScroll.UpdateScrollInfo();
		}
		if(GetVScroll())
		{
			if(!VerticalScroll.m_MaximumSetExternally)
			{
				VerticalScroll.SetMaximum(m_DisplayRectangle.Height - 1);
			}
			if(!VerticalScroll.m_LargeChangeSetExternally)
			{
				VerticalScroll.SetLargeChange(GetClientRectangle().Height);
			}
			if(!VerticalScroll.m_SmallChangeSetExternally)
			{
				VerticalScroll.SetSmallChange(5);
			}
			if(-m_DisplayRectangle.Y >= VerticalScroll.GetMinimum() && -m_DisplayRectangle.Y < VerticalScroll.GetMaximum())
			{
				VerticalScroll.SetValue(-m_DisplayRectangle.Y);
			}

			VerticalScroll.UpdateScrollInfo();
		}
	}
	else
	{
		if(HorizontalScroll.IsVisible())
		{
			HorizontalScroll.SetValue(-m_DisplayRectangle.X);
		}
		else
		{
			ResetScrollProperties(&HorizontalScroll);
		}
		if(VerticalScroll.IsVisible())
		{
			VerticalScroll.SetValue(-m_DisplayRectangle.Y);
		}
		else
		{
			ResetScrollProperties(&VerticalScroll);
		}
	}
}

void ScrollableControl::ResetScrollProperties(ScrollProperties* const scrollProperties)
{
	assert(scrollProperties != nullptr);
	scrollProperties->Disable();
	scrollProperties->SetValue(0);
}

void ScrollableControl::WmVScroll(Message& m)
{
	// The lparam is handle of the sending scrollbar, or NULL when
	// the scrollbar sending the message is the "form" scrollbar...		
	if(m.lParam != IntPtr::Zero())
	{
		Control::WndProc(m);
		return;
	}

	Drawing::Rectangle client = GetClientRectangle();
	bool thumbTrack = LOWORD(m.wParam) != SB_THUMBTRACK;
	int pos = m_DisplayRectangle.Y;
	int oldValue = pos;

	int maxPos = -(client.Height - m_DisplayRectangle.Height);
	if(!HasAutoScroll()) maxPos = VerticalScroll.GetMaximum();
	
	switch(LOWORD(m.wParam))
	{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
		{
			pos = ScrollThumbPosition(SB_VERT);
			break;
		}
		case SB_LINEUP:
		{
			if(pos > 0) pos -= VerticalScroll.GetSmallChange();
			else pos = 0;
			break;
		}
		case SB_LINEDOWN:
		{
			if(pos < maxPos - VerticalScroll.GetSmallChange()) pos += VerticalScroll.GetSmallChange();
			else pos = maxPos;
			break;
		}
		case SB_PAGEUP:
		{
			if(pos > VerticalScroll.GetLargeChange()) pos -= VerticalScroll.GetLargeChange();
			else pos = 0;
			break;
		}
		case SB_PAGEDOWN:
		{
			if(pos < maxPos - VerticalScroll.GetLargeChange()) pos += VerticalScroll.GetLargeChange();
			else pos = maxPos;
			break;
		}
		case SB_TOP:
		{
			pos = 0;
			break;
		}
		case SB_BOTTOM:
		{
			pos = maxPos;
			break;
		}
	}

	if(GetScrollState(ScrollStateFullDrag) || thumbTrack)
	{
		SetScrollState(ScrollStateUserHasScrolled, true);
		SetDisplayRectLocation(m_DisplayRectangle.X, -pos);
		SyncScrollbars(HasAutoScroll());
	}

	WmOnScroll(m, oldValue, pos, ScrollOrientation::VerticalScroll);
}

void ScrollableControl::WmHScroll(Message& m)
{
	// The lparam is handle of the sending scrollbar, or NULL when
	// the scrollbar sending the message is the "form" scrollbar...
	if(m.lParam != IntPtr::Zero())
	{
		Control::WndProc(m);
		return;
	}

	Drawing::Rectangle client = GetClientRectangle();

	int pos = -m_DisplayRectangle.X;
	int oldValue = pos;
	int maxPos = -(client.Width - m_DisplayRectangle.Width);
	if(!HasAutoScroll()) maxPos = HorizontalScroll.GetMaximum();
	
	switch(LOWORD(m.wParam))
	{
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
		{
			pos = ScrollThumbPosition(SB_HORZ);
			break;
		}
		case SB_LINEUP:
		{
			if(pos > HorizontalScroll.GetSmallChange()) pos -= HorizontalScroll.GetSmallChange();
			else pos = 0;
			break;
		}
		case SB_LINEDOWN:
		{
			if(pos < maxPos - HorizontalScroll.GetSmallChange()) pos += HorizontalScroll.GetSmallChange();
			else pos = maxPos;
			break;
		}
		case SB_PAGEUP:
		{
			if(pos > HorizontalScroll.GetLargeChange()) pos -= HorizontalScroll.GetLargeChange();
			else pos = 0;
			break;
		}
		case SB_PAGEDOWN:
			if(pos < maxPos - HorizontalScroll.GetLargeChange()) pos += HorizontalScroll.GetLargeChange();
			else pos = maxPos;
			break;
		case SB_LEFT:
		{
			pos = 0;
			break;
		}
		case SB_RIGHT:
		{
			pos = maxPos;
			break;
		}
	}

	if(GetScrollState(ScrollStateFullDrag) || LOWORD(m.wParam) != SB_THUMBTRACK)
	{
		SetScrollState(ScrollStateUserHasScrolled, true);
		SetDisplayRectLocation(-pos, m_DisplayRectangle.Y);
		SyncScrollbars(HasAutoScroll());
	}

	WmOnScroll(m, oldValue, pos, ScrollOrientation::HorizontalScroll);
}

void ScrollableControl::WmOnScroll(Message& m, int oldValue, int value, ScrollOrientation scrollOrientation)
{
	ScrollEventType type = (ScrollEventType)LOWORD(m.wParam);
	if(type != ScrollEventType::EndScroll)
	{
		Dispatch("OnScroll", new ScrollEventArgs(type, oldValue, value, scrollOrientation));
	}
}

void ScrollableControl::WmSettingChange(Message& m)
{
	Control::WndProc(m);
	UpdateFullDrag();
}

void ScrollableControl::OnSetScrollPosition(Object* const sender, EventArgs* const e)
{
	if(!IsMirrored())
	{
		SendControlMessage(WM_HSCROLL, static_cast<int>(MAKELPARAM((IsRightToLeft() == RightToLeft::Yes) ? SB_RIGHT : SB_LEFT, 0)), 0);
	}
}

ScrollableControl::ScrollableControl()
	:
	Control(),
	m_UserAutoScrollMinSize(Size::Empty()),
	m_DisplayRectangle(Drawing::Rectangle::Empty()),
	m_ScrollMargin(Size::Empty()),
	m_RequestedScrollMargin(Size::Empty()),
	m_ScrollPosition(Point::Empty()),
	HorizontalScroll(this),
	VerticalScroll(this),
	m_ScrollState(0),
	m_ResetRTLHScrollValue(false),
	m_DockPadding(this)
{
	SetStyle(ControlStyles::ContainerControl, true);
	SetStyle(ControlStyles::AllPaintingInWmPaint, false);
	SetScrollState(ScrollStateAutoScrolling, false);
}

ScrollableControl::~ScrollableControl()
{

}

Drawing::Rectangle ScrollableControl::GetDisplayRectangle()
{
	auto rect = GetClientRectangle();
	if(!m_DisplayRectangle.IsEmpty())
	{
		rect.X = m_DisplayRectangle.X;
		rect.Y = m_DisplayRectangle.Y;

		if(GetHScroll()) rect.Width = m_DisplayRectangle.Width;
		if(GetVScroll()) rect.Height = m_DisplayRectangle.Height;
	}
	//rect.Deflate()

	return rect;
}

bool ScrollableControl::GetScrollState(int bit) const noexcept
{
	return (bit & m_ScrollState) == bit;
}

void ScrollableControl::SetScrollState(int bit, bool value)
{
	if(value) m_ScrollState |= bit;
	else m_ScrollState &= (~bit);
}

bool ScrollableControl::GetHScroll() const noexcept
{
	return GetScrollState(ScrollStateHScrollVisible);
}

void ScrollableControl::SetHScroll(bool value)
{
	SetScrollState(ScrollStateHScrollVisible, value);
}

bool ScrollableControl::GetVScroll() const noexcept
{
	return GetScrollState(ScrollStateVScrollVisible);
}

void ScrollableControl::SetVScroll(bool value)
{
	SetScrollState(ScrollStateVScrollVisible, value);
}

void ScrollableControl::SetDisplayRectLocation(int x, int y)
{
	int xDelta = 0;
	int yDelta = 0;

	Drawing::Rectangle client = GetClientRectangle();
	//VSWhidbey 141644 - The DisplayRect property modifies
	// the returned rect to include padding.  We don't want to
	// include this padding in our adjustment of the DisplayRect
	// because it interferes with the scrolling.
	Drawing::Rectangle displayRectangle = m_DisplayRectangle;
	int minX = (std::min)(client.Width - displayRectangle.Width, 0);
	int minY = (std::min)(client.Height - displayRectangle.Height, 0);

	if(x > 0) x = 0;
	if(y > 0) y = 0;
	if(x < minX) x = minX;
	if(y < minY) y = minY;
	
	if(displayRectangle.X != x) xDelta = x - displayRectangle.X;
	if(displayRectangle.Y != y) yDelta = y - displayRectangle.Y;
	
	m_DisplayRectangle.X = x;
	m_DisplayRectangle.Y = y;

	if(xDelta != 0 || yDelta != 0 && IsHandleCreated())
	{
		Drawing::Rectangle cr = GetClientRectangle();
		RECT rcClip = RECT(cr.X, cr.Y, cr.Width, cr.Height);
		RECT rcUpdate = RECT(cr.X, cr.Y, cr.Width, cr.Height);
		ScrollWindowEx(GetHandle(), xDelta, yDelta,
										 nullptr,
										 &rcClip,
										 nullptr,
										 &rcUpdate,
										 SW_INVALIDATE | SW_ERASE | SW_SCROLLCHILDREN);
	}

	// Force child controls to update bounds.
	for(const auto& c : Controls)
	{
		if(c != nullptr && c->IsHandleCreated())
		{
			c->UpdateBounds();
		}
	}
}

CreateParams* ScrollableControl::CreateParameters()
{
	CreateParams* cp = Control::CreateParameters();

	if(GetHScroll() || HorizontalScroll.IsVisible()) cp->Style |= WS_HSCROLL;
	else cp->Style &= WS_HSCROLL;

	if(GetVScroll() || VerticalScroll.IsVisible()) cp->Style |= WS_VSCROLL;
	else cp->Style &= WS_VSCROLL;

	return cp;
}

void ScrollableControl::WndProc(Message& m)
{
	switch(m.Msg)
	{
		case WM_VSCROLL:
		{
			WmVScroll(m);
			break;
		}
		case WM_HSCROLL:
		{
			WmHScroll(m);
			break;
		}
		case WM_SETTINGCHANGE:
		{
			WmSettingChange(m);
			break;
		}
		default:
		{
			Control::WndProc(m);
			break;
		}
	}
}

bool ScrollableControl::HasAutoScroll()
{
	return GetScrollState(ScrollStateAutoScrolling);
}

void ScrollableControl::SetAutoScroll(bool value)
{
	if(value) UpdateFullDrag();

	SetScrollState(ScrollStateAutoScrolling, true);
}

Size ScrollableControl::GetAutoScrollMargin() const noexcept
{
	return m_RequestedScrollMargin;
}

void ScrollableControl::SetAutoScrollMargin(int x, int y)
{
	if(x < 0) x = 0;
	if(y < 0) y = 0;

	if(x != m_RequestedScrollMargin.Width || y != m_RequestedScrollMargin.Height)
	{
		m_RequestedScrollMargin.Width = x;
		m_RequestedScrollMargin.Height = y;
		
		if(HasAutoScroll())	PerformLayout();
	}
}

void ScrollableControl::SetAutoScrollMargin(Size s)
{
	if(s.Width < 0 || s.Height < 0) throw ArgumentOutOfRangeException("AutoScrollMargin", "InvalidArgument");

	SetAutoScrollMargin(s.Width, s.Height);
}

