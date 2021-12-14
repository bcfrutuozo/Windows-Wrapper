#pragma once

#include "IntPtr.h"
#include "SafeNativeMethods.h"
#include "UnsafeNativeMethods.h"

namespace Microsoft::Win32::NativeMethods
{
	struct PDH_FMT_COUNTERVALUE
	{
		int CStatus = 0;
		double data = 0;
	};

	struct PDH_RAW_COUNTER
	{
		int CStatus = 0;
		long TimeStamp = 0;
		long FirstValue = 0;
		long SecondValue = 0;
		int MultiCount = 0;
	};

	struct SECURITY_ATTRIBUTES
	{
		int nLength = 12;
		IntPtr lpSecurityDescriptor = IntPtr::Zero();
		bool bInheritHandle = false;
	};

	struct STARTUPINFO
	{
		int cb;
		IntPtr lpReserved = IntPtr::Zero();
		IntPtr lpDesktop = IntPtr::Zero();
		IntPtr lpTitle = IntPtr::Zero();
		int dwX = 0;
		int dwY = 0;
		int dwXSize = 0;
		int dwYSize = 0;
		int dwXCountChars = 0;
		int dwYCountChars = 0;
		int dwFillAttribute = 0;
		int dwFlags = 0;
		short wShowWindow = 0;
		short cbReserved2 = 0;
		IntPtr lpReserved2 = IntPtr::Zero();
		IntPtr hStdInput = IntPtr::Zero();
		IntPtr hStdOutput = IntPtr::Zero();
		IntPtr hStdError = IntPtr::Zero();

		STARTUPINFO();

		// Manage object destruction calling ::CloseHandle()
		void Dispose();
	};

	struct TEXTMETRIC
	{
		int tmHeight = 0;
		int tmAscent = 0;
		int tmDescent = 0;
		int tmInternalLeading = 0;
		int tmExternalLeading = 0;
		int tmAveCharWidth = 0;
		int tmMaxCharWidth = 0;
		int tmWeight = 0;
		int tmOverhang = 0;
		int tmDigitizedAspectX = 0;
		int tmDigitizedAspectY = 0;
		char tmFirstChar = '\0';
		char tmLastChar = '\0';
		char tmDefaultChar = '\0';
		char tmBreakChar = '\0';
		unsigned char tmItalic = 0;
		unsigned char tmUnderlined = 0;
		unsigned char tmStruckOut = 0;
		unsigned char tmPitchAndFamily = 0;
		unsigned char tmCharSet = 0;
	};

	struct USEROBJECTFLAGS
	{
		int fInherit = 0;
		int fReserved = 0;
		int dwFlags;
	};

	static constexpr int DEFAULT_GUI_FONT = 17;
	static constexpr int SM_CYSCREEN = 1;

	static constexpr IntPtr NullHandleRef = IntPtr::Zero();
	static constexpr IntPtr INVALID_HANDLE_VALUE = IntPtr(-1);

	static constexpr int GENERIC_READ = ((int)0x80000000);
	static constexpr int GENERIC_WRITE = (0x40000000);

	static constexpr int FILE_SHARE_READ = 0x00000001;
	static constexpr int FILE_SHARE_WRITE = 0x00000002;
	static constexpr int FILE_SHARE_DELETE = 0x00000004;

	static constexpr int S_OK = 0x0;
	static constexpr int E_ABORT = unchecked((int)0x80004004);
	static constexpr int E_NOTIMPL = unchecked((int)0x80004001);

	static constexpr int CREATE_ALWAYS = 2;

	static constexpr int FILE_ATTRIBUTE_NORMAL = 0x00000080;

	static constexpr int STARTF_USESTDHANDLES = 0x00000100;

	static constexpr int STD_INPUT_HANDLE = -10;
	static constexpr int STD_OUTPUT_HANDLE = -11;
	static constexpr int STD_ERROR_HANDLE = -12;

	static constexpr int STILL_ACTIVE = 0x00000103;
	static constexpr int SW_HIDE = 0;

	static constexpr int WAIT_OBJECT_0 = 0x00000000;
	static constexpr int WAIT_FAILED = unchecked((int)0xFFFFFFFF);
	static constexpr int WAIT_TIMEOUT = 0x00000102;
	static constexpr int WAIT_ABANDONED = 0x00000080;
	static constexpr int WAIT_ABANDONED_0 = WAIT_ABANDONED;

	static constexpr int MOVEFILE_REPLACE_EXISTING = 0x00000001;

	static constexpr int ERROR_CLASS_ALREADY_EXISTS = 1410;
	static constexpr int ERROR_NONE_MAPPED = 1332;
	static constexpr int ERROR_INSUFFICIENT_BUFFER = 122;

	static constexpr int ERROR_INVALID_NAME = 0x7B; //123

	static constexpr int ERROR_PROC_NOT_FOUND = 127;
	static constexpr int ERROR_BAD_EXE_FORMAT = 193;
	static constexpr int ERROR_EXE_MACHINE_TYPE_MISMATCH = 216;
	static constexpr int MAX_PATH = 260;

	bool CreatePipe(IntPtr*  readPipe, IntPtr* writePipe, SECURITY_ATTRIBUTES pipeAttributes, int size);
	bool CreateProcess(const std::string& applicationName,
					   const std::string& commandLine,
					   SECURITY_ATTRIBUTES processAttributes,
					   SECURITY_ATTRIBUTES threadAttributes,
					   bool inheritHandles,
					   int creationFlags,
					   const std::string& currentDirectory,
					   STARTUPINFO startupInfo,
					   PROCESS_INFORMATION processInformation);

	bool GetExitCodeProcess(IntPtr processHandle, int* exitCode);
	bool GetProcessTimes(IntPtr handle, long* creation, long* exit, long* kernel, long* user);
	IntPtr GetStdHandle(int whichHandle);
	bool GetThreadTimes(IntPtr handle, long* creation, long* exit, long* kernel, long* user);
}