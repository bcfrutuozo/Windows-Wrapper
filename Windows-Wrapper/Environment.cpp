#include "Environment.h"

#include <Windows.h>

OperatingSystem Environment::m_OS = OperatingSystem();
bool Environment::m_IsWindows8OrAbove = false;
bool Environment::m_CheckedOSWin8OrAbove = false;

void Environment::_Exit(int exitCode)
{
	exit(exitCode);
}

OperatingSystem Environment::OSVersion()
{
	if(!m_OS.Loaded)
	{
		OSVERSIONINFO osvi = OSVERSIONINFO();
		GetVersionEx(&osvi);

		typedef NTSTATUS(NTAPI* RtlGetVersionFunction)(LPOSVERSIONINFO);
		OSVERSIONINFOEX osviEx = { sizeof(OSVERSIONINFOEX), 0, 0, 0, 0, {'\0'}, 0, 0, 0, 0, 0 };

		HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
		if(ntdll)
		{
			RtlGetVersionFunction pRtlGetVersion = reinterpret_cast<RtlGetVersionFunction>(GetProcAddress(ntdll, "RtlGetVersion"));

			if(pRtlGetVersion)
			{
				pRtlGetVersion((LPOSVERSIONINFO)&osviEx);
			}
		}

		Version v = Version(osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, (osviEx.wServicePackMajor << 16) | osviEx.wServicePackMinor);
		m_OS = OperatingSystem(PlatformID::Win32NT, std::string(osvi.szCSDVersion));
	}

	return m_OS;
}

bool Environment::IsWindows8OrAbove()
{
	if(!m_CheckedOSWin8OrAbove)
	{
		OperatingSystem OS = Environment::OSVersion();
		m_IsWindows8OrAbove = (OS.GetPlatform() == PlatformID::Win32NT &&
							   ((OS.GetVersion().GetMajor() == 6 && OS.GetVersion().GetMinor() >= 2) || (OS.GetVersion().GetMajor() > 6)));
		m_CheckedOSWin8OrAbove = true;
	}

	return m_IsWindows8OrAbove;
}

void Environment::Exit(int exitCode)
{
	_Exit(exitCode);
}

std::string Environment::CurrentDirectory()
{
	char buffer[257];
	GetCurrentDirectory(sizeof(buffer), buffer);
	return std::string(buffer);
}

std::string Environment::SystemDirectory()
{
	char buffer[257];
	GetSystemDirectory(buffer, sizeof(buffer));
	return std::string(buffer);
}

std::string Environment::InternalWindowsDirectory()
{
	char buffer[257];
	GetWindowsDirectory(buffer, sizeof(buffer));
	return std::string(buffer);
}

std::string Environment::MachineName()
{
	char buffer[MaxMachineNameLength];
	DWORD maxSize = MaxMachineNameLength;
	GetComputerName(buffer, &maxSize);
	return std::string(buffer);
}

bool Environment::Is64BitProcess()
{
#ifdef _WIN64
	return true;
#else
	return false;
#endif
}

bool Environment::Is64OperatingSystem()
{
#ifdef _WIN64
	return true;
#else
	return false;
#endif
}
