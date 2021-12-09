#pragma once

#include <Windows.h>

enum class ScrollEventType
{
	SmallDecrement = SB_LINELEFT,
	SmallIncrement = SB_LINERIGHT,
	LargeDecrement = SB_PAGELEFT,
	LargeIncrement = SB_PAGERIGHT,
	ThumbPosition = SB_THUMBPOSITION,
	ThumbTrack = SB_THUMBTRACK,
	First = SB_LEFT,
	Last = SB_RIGHT,
	EndScroll = SB_ENDSCROLL
};