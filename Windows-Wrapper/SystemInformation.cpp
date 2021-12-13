#include "SystemInformation.h"

bool SystemInformation::CheckMultiMonitorSupport = false;
bool SystemInformation::MultiMonitorSupport = false;
bool SystemInformation::CheckNativeMouseWheelSupport = false;
bool SystemInformation::NativeMouseWheelSupport = true;
bool SystemInformation::HighContrast = false;
bool SystemInformation::SystemEventsAttached = false;
bool SystemInformation::SystemEventsDirty = true;
IntPtr SystemInformation::ProcessWinStation = IntPtr::Zero();
bool SystemInformation::IsUserInteractive = false;
PowerStatus SystemInformation::PowerStatus;

SystemInformation::SystemInformation()
{}

void SystemInformation::EnsureSystemEvents()
{
	if(!SystemEventsAttached)
	{
		//SystemEvents.UserPreferenceChanged = new UserPreferenceChangedEventHandler(SystemInformation.OnUserPreferenceChanged);
		SystemEventsAttached = true;
	}
}

void SystemInformation::OnUserPreferenceChanged(Object* const sender, UserPreferenceChangedEventArgs* const args)
{
	SystemEventsDirty = true;
}

std::string SystemInformation::GetUser() noexcept
{
	char username[257];
	DWORD username_len = 257;
	GetUserName(username, &username_len);
	return std::string(username);
}

bool SystemInformation::IsDragFullWindowsEnabled() noexcept
{
	int data = 0;
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &data, 0);
	return data != 0;
}

bool SystemInformation::IsHighContrastEnabled() noexcept
{
	EnsureSystemEvents();

	if(SystemEventsDirty)
	{
		HIGHCONTRAST data = { 0 };
		data.cbSize = sizeof(data);
		data.dwFlags = 0;
		data.lpszDefaultScheme = 0;

		if(SystemParametersInfo(SPI_GETHIGHCONTRAST, data.cbSize, &data, 0)) HighContrast = (data.dwFlags & HCF_HIGHCONTRASTON) != 0;
		else HighContrast = false;

		SystemEventsDirty = false;
	}

	return HighContrast;
}

bool SystemInformation::IsUserInteractiveModeEnabled() noexcept
{
	return false;
}

bool SystemInformation::IsFlatMenuEnabled() noexcept
{
	return false;
}

Size SystemInformation::MinWindowTrackSize()
{
	return Size(GetSystemMetrics(SM_CXMINTRACK), GetSystemMetrics(SM_CYMINTRACK));
}

Size SystemInformation::MaxWindowTrackSize()
{
	return Size(GetSystemMetrics(SM_CXMAXTRACK), GetSystemMetrics(SM_CYMAXTRACK));
}