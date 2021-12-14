#pragma once

enum class ScrollEventType
{
	SmallDecrement = NativeMethods::SB_LINELEFT,
	SmallIncrement = NativeMethods::SB_LINERIGHT,
	LargeDecrement = NativeMethods::SB_PAGELEFT,
	LargeIncrement = NativeMethods::SB_PAGERIGHT,
	ThumbPosition = NativeMethods::SB_THUMBPOSITION,
	ThumbTrack = NativeMethods::SB_THUMBTRACK,
	First = NativeMethods::SB_LEFT,
	Last = NativeMethods::SB_RIGHT,
	EndScroll = NativeMethods::SB_ENDSCROLL
};