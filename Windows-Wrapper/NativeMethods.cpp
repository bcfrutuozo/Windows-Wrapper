#include "NativeMethods.h"

#include <Windows.h>

namespace Microsoft::Win32
{
	NativeMethods::STARTUPINFO::STARTUPINFO()
	{
		// Gets the size of the native STARTUPINFO class struct
		// to avoid existing overload on the wrapper (classes inheritances)
		cb = sizeof(::STARTUPINFO);
	}

	void NativeMethods::STARTUPINFO::Dispose()
	{
        // Close the handles created for child process
        if(hStdInput != IntPtr::Zero())
        {
            CloseHandle(hStdInput.ToPointer());
            hStdInput = IntPtr::Zero();
        }

        if(hStdOutput != IntPtr::Zero())
        {
            CloseHandle(hStdOutput.ToPointer());
            hStdOutput = IntPtr::Zero();
        }

        if(hStdError != IntPtr::Zero())
        {
            CloseHandle(hStdError.ToPointer());
            hStdError = IntPtr::Zero();
        }
	}
}

bool Microsoft::Win32::NativeMethods::CreatePipe(IntPtr* hReadPipe, IntPtr* hWritePipe, SECURITY_ATTRIBUTES lpPipeAttributes, int nSize)
{
    void* pRead;
    void* pWrite;

    ::SECURITY_ATTRIBUTES sa;
    sa.nLength = lpPipeAttributes.nLength;
    sa.lpSecurityDescriptor = lpPipeAttributes.lpSecurityDescriptor.ToPointer();
    sa.bInheritHandle = lpPipeAttributes.bInheritHandle;

    bool ret = ::CreatePipe(&pRead, &pWrite, &sa, nSize);

    *hReadPipe = pRead;
    *hWritePipe = pWrite;

    return ret;
}

bool Microsoft::Win32::NativeMethods::GetExitCodeProcess(IntPtr processHandle, int* exitCode)
{
    DWORD value = 0;
    bool ret = ::GetExitCodeProcess(processHandle.ToPointer(), &value);
    *exitCode = value;
    return ret;
}

bool Microsoft::Win32::NativeMethods::GetProcessTimes(IntPtr handle, long* creation, long* exit, long* kernel, long* user)
{
    FILETIME lpCreation;
    FILETIME lpExit;
    FILETIME lpKernel;
    FILETIME lpUser;

    bool ret = ::GetProcessTimes(handle.ToPointer(), &lpCreation, &lpExit, &lpKernel, &lpUser);

    *creation = ULARGE_INTEGER { lpCreation.dwLowDateTime, lpCreation.dwHighDateTime }.QuadPart;
    *exit = ULARGE_INTEGER{ lpExit.dwLowDateTime, lpExit.dwHighDateTime }.QuadPart;
    *kernel = ULARGE_INTEGER{ lpKernel.dwLowDateTime, lpKernel.dwHighDateTime }.QuadPart;
    *user = ULARGE_INTEGER{ lpUser.dwLowDateTime, lpUser.dwHighDateTime }.QuadPart;

    return ret;
}

bool Microsoft::Win32::NativeMethods::GetThreadTimes(IntPtr handle, long* creation, long* exit, long* kernel, long* user)
{
    FILETIME lpCreation;
    FILETIME lpExit;
    FILETIME lpKernel;
    FILETIME lpUser;

    bool ret = ::GetThreadTimes(handle.ToPointer(), &lpCreation, &lpExit, &lpKernel, &lpUser);

    *creation = ULARGE_INTEGER{ lpCreation.dwLowDateTime, lpCreation.dwHighDateTime }.QuadPart;
    *exit = ULARGE_INTEGER{ lpExit.dwLowDateTime, lpExit.dwHighDateTime }.QuadPart;
    *kernel = ULARGE_INTEGER{ lpKernel.dwLowDateTime, lpKernel.dwHighDateTime }.QuadPart;
    *user = ULARGE_INTEGER{ lpUser.dwLowDateTime, lpUser.dwHighDateTime }.QuadPart;

    return ret;
}

IntPtr Microsoft::Win32::NativeMethods::GetStdHandle(int whichHandle)
{
    return ::GetStdHandle(whichHandle);
}
