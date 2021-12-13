#pragma once

#include "Object.h"
#include "IntPtr.h"
#include "PowerStatus.h"
#include "UserPreferenceChangedEventArgs.h"
#include "Size.h"

#include <string>

class SystemInformation : public Object
{
private:

    static bool CheckMultiMonitorSupport;
    static bool MultiMonitorSupport;
    static bool CheckNativeMouseWheelSupport;
    static bool NativeMouseWheelSupport;
    static bool HighContrast;
    static bool SystemEventsAttached;
    static bool SystemEventsDirty;
    static IntPtr ProcessWinStation;
    static bool IsUserInteractive;
    static PowerStatus PowerStatus;

    static constexpr int DefaultMouseWheelScrollLines = 3;

	SystemInformation();

    static void EnsureSystemEvents();
    static void OnUserPreferenceChanged(Object* const sender, UserPreferenceChangedEventArgs* const args);

public:

    static std::string GetUser() noexcept;
    static bool IsDragFullWindowsEnabled() noexcept;
    static bool IsHighContrastEnabled() noexcept;
    static bool IsUserInteractiveModeEnabled() noexcept;
    static bool IsFlatMenuEnabled() noexcept;
    static Size MinWindowTrackSize();
    static Size MaxWindowTrackSize();
};

