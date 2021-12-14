#include "SafeNativeMethods.h"
#include "NativeMethods.h"

#include <Pdh.h>
#include <Windows.h>

// Undeclare all use WinAPI macros to wrapper them up
#undef CreateSemaphore
#undef GetComputerName
#undef GetTextMetrics
#undef FormatMessage
#undef InterlockedCompareExchange
#undef LoadLibrary
#undef MessageBox
#undef OpenSemaphore
#undef OutputDebugString
#undef RegisterWindowMessage

bool Microsoft::Win32::SafeNativeMethods::CloseHandle(IntPtr handle)
{
	return ::CloseHandle(handle);
}

IntPtr Microsoft::Win32::SafeNativeMethods::CreateSemaphore(NativeMethods::SECURITY_ATTRIBUTES* lpSecurityAttributes, int initialCount, int maximumCount, const std::string& name)
{
	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle = lpSecurityAttributes->bInheritHandle;
	sa.lpSecurityDescriptor = lpSecurityAttributes->lpSecurityDescriptor;
	sa.nLength = lpSecurityAttributes->nLength;

#ifdef UNICODE
	return ::CreateSemaphoreW(&sa, initialCount, maximumCount, name.c_str());
#else
	return ::CreateSemaphoreA(&sa, initialCount, maximumCount, name.c_str());
#endif
	return IntPtr();
}

int Microsoft::Win32::SafeNativeMethods::FormatFromRawValue(unsigned int counterType, unsigned int format, long& timeBase, NativeMethods::PDH_RAW_COUNTER* rawValue1, NativeMethods::PDH_RAW_COUNTER* rawValue2, NativeMethods::PDH_FMT_COUNTERVALUE* fmtValue)
{
	PDH_RAW_COUNTER* pRaw1;
	pRaw1->CStatus = rawValue1->CStatus;
	pRaw1->FirstValue = rawValue1->FirstValue;
	pRaw1->MultiCount = rawValue1->MultiCount;
	pRaw1->SecondValue = rawValue1->SecondValue;
	pRaw1->SecondValue = rawValue1->TimeStamp;

	PDH_RAW_COUNTER* pRaw2;
	pRaw2->CStatus = rawValue2->CStatus;
	pRaw2->FirstValue = rawValue2->FirstValue;
	pRaw2->MultiCount = rawValue2->MultiCount;
	pRaw2->SecondValue = rawValue2->SecondValue;
	pRaw2->SecondValue = rawValue2->TimeStamp;

	PDH_FMT_COUNTERVALUE pFmt;

	LONGLONG tb = timeBase;

	int ret = ::PdhFormatFromRawValue(counterType, format, &tb, pRaw1, pRaw2, &pFmt);

	fmtValue->CStatus = pFmt.CStatus;
	fmtValue->data = pFmt.doubleValue;

	return ret;
}

int Microsoft::Win32::SafeNativeMethods::FormatMessage(int flags, IntPtr source, unsigned int message, int languageId, std::string& buffer, int size, IntPtr arguments[])
{
	va_list largs;
	va_start(largs, arguments);

#ifdef UNICODE
	wchar_t* bufferchar = new wchar_t[size];

	int ret = ::FormatMessageW(flags, &source, message, languageId, bufferchar, size, &largs);
	buffer = std::wstring(bufferchar);
	delete bufferchar;
#else
	char* bufferchar = new char[size];

	int ret = ::FormatMessageA(flags, &source, message, languageId, bufferchar, size, &largs);
	buffer = std::string(bufferchar);
	delete bufferchar;
#endif

	return ret;
}

bool Microsoft::Win32::SafeNativeMethods::FreeLibrary(IntPtr hModule)
{
	return ::FreeLibrary((HMODULE)hModule.ToPointer());
}

bool Microsoft::Win32::SafeNativeMethods::GetComputerName(std::string& buffer)
{
#ifdef UNICODE
	wchar_t cName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size;
	bool ret = GetComputerNameW(cName, &size);

	buffer = std::string(cName);

#else
	char cName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size;
	bool ret = GetComputerNameA(cName, &size);

	buffer = std::string(cName);
#endif

	return ret;
}

IntPtr Microsoft::Win32::SafeNativeMethods::GetStockObject(int nIndex)
{
	return ::GetStockObject(nIndex);
}

bool Microsoft::Win32::SafeNativeMethods::GetTextMetrics(IntPtr hDC, Microsoft::Win32::NativeMethods::TEXTMETRIC* ts)
{
	TEXTMETRIC tm;

#ifdef UNICODE
	bool ret = ::GetTextMetricsW((HDC)hDC.ToPointer(), tm);
#else
	bool ret = ::GetTextMetricsA((HDC)hDC.ToPointer(), &tm);
#endif

	ts->tmHeight = tm.tmHeight;
	ts->tmAscent = tm.tmAscent;
	ts->tmDescent = tm.tmDescent;
	ts->tmInternalLeading = tm.tmInternalLeading;
	ts->tmExternalLeading = tm.tmExternalLeading;
	ts->tmAveCharWidth = tm.tmAveCharWidth;
	ts->tmMaxCharWidth = tm.tmMaxCharWidth;
	ts->tmWeight = tm.tmWeight;
	ts->tmOverhang = tm.tmOverhang;
	ts->tmDigitizedAspectX = tm.tmDigitizedAspectX;
	ts->tmDigitizedAspectY = tm.tmDigitizedAspectY;
	ts->tmFirstChar = tm.tmFirstChar;
	ts->tmLastChar = tm.tmLastChar;
	ts->tmDefaultChar = tm.tmDefaultChar;
	ts->tmBreakChar = tm.tmBreakChar;
	ts->tmItalic = tm.tmItalic;
	ts->tmUnderlined = tm.tmUnderlined;
	ts->tmStruckOut = tm.tmStruckOut;
	ts->tmPitchAndFamily = tm.tmPitchAndFamily;
	ts->tmCharSet = tm.tmCharSet;

	return ret;
}

int Microsoft::Win32::SafeNativeMethods::InterlockedCompareExchange(IntPtr destination, int exchange, int compare)
{
	::_InterlockedCompareExchange((long*)destination.ToPointer(), exchange, compare);
}

bool Microsoft::Win32::SafeNativeMethods::IsWoW64Process(IntPtr hProcess, bool& is64Process)
{
	int value;
	bool ret = ::IsWow64Process((HANDLE)hProcess.ToPointer(), &value);
	is64Process = value;
	return ret;
}

IntPtr Microsoft::Win32::SafeNativeMethods::LoadLibrary(const std::string& fileName)
{
#ifdef UNICODE
	return ::LoadLibraryW(fileName.c_str());
#else
	return ::LoadLibraryA(fileName.c_str());
#endif
}

int Microsoft::Win32::SafeNativeMethods::MessageBox(IntPtr hWnd, const std::string& text, const std::string& caption, int type)
{
#ifdef UNICODE
	return = ::MessageBoxW((HWND)hWnd.ToPointer(), text.c_str(), caption.c_str(), type);
#else
	return ::MessageBoxA((HWND)hWnd.ToPointer(), text.c_str(), caption.c_str(), type);
#endif
}

IntPtr Microsoft::Win32::SafeNativeMethods::OpenSemaphore(int desiredAccess, bool inheritHandle, const std::string& name)
{
#ifdef UNICODE
	return = ::OpenSemaphoreW(desiredAccess, inheritHandle, name.c_str());
#else
	return ::OpenSemaphoreA(desiredAccess, inheritHandle, name.c_str());
#endif
}

void Microsoft::Win32::SafeNativeMethods::OutputDebugString(const std::string& message)
{
#ifdef UNICODE
	::GetTextMetricsW(message.c_str());
#else
	::OutputDebugStringA(message.c_str());
#endif
}

bool Microsoft::Win32::SafeNativeMethods::QueryPerformanceCounter(long& value)
{
	LARGE_INTEGER li;

	bool ret = ::QueryPerformanceCounter(&li);

	value = li.QuadPart;

	return ret;
}

bool Microsoft::Win32::SafeNativeMethods::QueryPerformanceFrequency(long& value)
{
	LARGE_INTEGER li;

	bool ret = ::QueryPerformanceFrequency(&li);

	value = li.QuadPart;

	return ret;
}

int Microsoft::Win32::SafeNativeMethods::RegisterWindowMessage(const std::string& message)
{
#ifdef UNICODE
	return ::RegisterWindowMessageW(message.c_str());
#else
	return ::RegisterWindowMessageA(message.c_str());
#endif
}

bool Microsoft::Win32::SafeNativeMethods::ReleaseSemaphore(IntPtr handle, int releaseCount, int& previousCount)
{
	LONG pCount;
	bool ret = ::ReleaseSemaphore(handle.ToPointer(), releaseCount, &pCount);
	previousCount = pCount;
	return ret;
}
