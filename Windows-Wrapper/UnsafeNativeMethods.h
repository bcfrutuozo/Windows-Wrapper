#pragma once

#include "IntPtr.h"

namespace Microsoft::Win32::NativeMethods
{
	class USEROBJECTFLAGS;
}

namespace Microsoft::Win32::UnsafeNativeMethods
{
	IntPtr GetDC(IntPtr hWnd);
	IntPtr GetModuleHandle(const std::string& modName);
	IntPtr GetProcessWindowStation();
	IntPtr GetStdHandle(int type);
	int GetSystemMetrics(int nIndex);
	bool GetUserObjectInformation(IntPtr hObj, int nIndex, NativeMethods::USEROBJECTFLAGS* pvBuffer, int nLength, int& lpnLengthNeeded);
	int ReleaseDC(IntPtr hWnd, IntPtr hDC);
	IntPtr SelectObject(IntPtr hdc, IntPtr hObject);
}