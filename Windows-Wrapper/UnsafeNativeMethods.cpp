#include "UnsafeNativeMethods.h"
#include "NativeMethods.h"

#include <Windows.h>
#undef GetModuleHandle
#undef GetUserObjectInformation

IntPtr Microsoft::Win32::UnsafeNativeMethods::GetDC(IntPtr hWnd)
{
	return ::GetDC((HWND)hWnd.ToPointer());
}

IntPtr Microsoft::Win32::UnsafeNativeMethods::GetModuleHandle(const std::string& modName)
{
#ifdef UNICODE
	return ::GetModuleHandleW(modName.c_str());
#else
	return ::GetModuleHandleA(modName.c_str());
#endif
}

IntPtr Microsoft::Win32::UnsafeNativeMethods::GetProcessWindowStation()
{
	return ::GetProcessWindowStation();
}

IntPtr Microsoft::Win32::UnsafeNativeMethods::GetStdHandle(int type)
{
	return ::GetStdHandle(type);
}

int Microsoft::Win32::UnsafeNativeMethods::GetSystemMetrics(int nIndex)
{
	return ::GetSystemMetrics(nIndex);
}

bool Microsoft::Win32::UnsafeNativeMethods::GetUserObjectInformation(IntPtr hObj, int nIndex, NativeMethods::USEROBJECTFLAGS* pvBuffer, int nLength, int& lpnLengthNeeded)
{
	USEROBJECTFLAGS uof;
	uof.dwFlags = pvBuffer->dwFlags;
	uof.fInherit = pvBuffer->fInherit;
	uof.fReserved = pvBuffer->fReserved;

	DWORD val;

#ifdef UNICODE
	bool ret = ::GetUserObjectInformationW(hObj.ToPointer(), nIndex, &uof, nLength, &val);
#else
	bool ret = ::GetUserObjectInformationA(hObj.ToPointer(), nIndex, &uof, nLength, &val);
#endif

	lpnLengthNeeded = val;

	return ret;
}

int Microsoft::Win32::UnsafeNativeMethods::ReleaseDC(IntPtr hWnd, IntPtr hDC)
{
	return ::ReleaseDC((HWND)hWnd.ToPointer(), (HDC)hDC.ToPointer());
}

IntPtr Microsoft::Win32::UnsafeNativeMethods::SelectObject(IntPtr hDC, IntPtr hObject)
{
	return ::SelectObject((HDC)hDC.ToPointer(), (HGDIOBJ)hObject.ToPointer());
}