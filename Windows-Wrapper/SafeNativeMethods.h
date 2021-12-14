#pragma once

#include "IntPtr.h"

namespace Microsoft::Win32::NativeMethods
{
	class PDH_RAW_COUNTER;
	class PDH_FMT_COUNTERVALUE;
	class SECURITY_ATTRIBUTES;
	class TEXTMETRIC;
}

namespace Microsoft::Win32::SafeNativeMethods
{
	struct PROCESS_INFORMATION
	{
		IntPtr hProcess = IntPtr::Zero();
		IntPtr hThread = IntPtr::Zero();
		int dwProcessId = 0;
		int dwThreadId = 0;
	};

	static constexpr int MB_RIGHT = 0x00080000;
	static constexpr int MB_RTLREADING = 0x00100000;

	static constexpr int FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x00000100,
	static constexpr int FORMAT_MESSAGE_IGNORE_INSERTS = 0x00000200,
	static constexpr int FORMAT_MESSAGE_FROM_STRING = 0x00000400,
	static constexpr int FORMAT_MESSAGE_FROM_SYSTEM = 0x00001000,
	static constexpr int FORMAT_MESSAGE_ARGUMENT_ARRAY = 0x00002000;

	static constexpr int ERROR_INSUFFICIENT_BUFFER = 0x7A;

	bool CloseHandle(IntPtr handle);
	IntPtr CreateSemaphore(NativeMethods::SECURITY_ATTRIBUTES* lpSecurityAttributes, int initialCount, int maximumCount, const std::string& name);
	int FormatFromRawValue(unsigned int counterType, unsigned int format, long& timeBase, NativeMethods::PDH_RAW_COUNTER* rawValue1, NativeMethods::PDH_RAW_COUNTER* rawValue2, NativeMethods::PDH_FMT_COUNTERVALUE* fmtValue);
	int FormatMessage(int flags, IntPtr source, unsigned int message, int languageId, std::string& buffer, int size, IntPtr arguments[]);
	bool FreeLibrary(IntPtr hModule);
	bool GetComputerName(std::string& buffer);
	IntPtr GetStockObject(int nIndex);
	bool GetTextMetrics(IntPtr hDC, NativeMethods::TEXTMETRIC* ts);
	int InterlockedCompareExchange(IntPtr destination, int exchange, int compare);
	bool IsWoW64Process(IntPtr hProcess, bool& is64Process);
	IntPtr LoadLibrary(const std::string& fileName);
	int MessageBox(IntPtr hWnd, const std::string& text, const std::string& caption, int type);
	IntPtr OpenSemaphore(int desiredAccess, bool inheritHandle, const std::string& name);
	void OutputDebugString(const std::string& message);
	bool QueryPerformanceCounter(long& value);
	bool QueryPerformanceFrequency(long& value);
	int RegisterWindowMessage(const std::string& message);
	bool ReleaseSemaphore(IntPtr handle, int releaseCount, int& previousCount);
}