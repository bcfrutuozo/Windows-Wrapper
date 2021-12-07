#pragma once

#include "IntPtr.h"

class IWin32Window
{
public:

	IntPtr Handle;

	constexpr IWin32Window() : Handle((unsigned long)0L) { }
};