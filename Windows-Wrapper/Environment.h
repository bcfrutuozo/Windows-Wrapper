#pragma once

#include "Object.h"
#include "OperatingSystem.h"

class Environment : public Object
{
private:

    // Assume the following constants include the terminating '\0' - use <, not <=
    static constexpr int MaxEnvVariableValueLength = 32767;  // Maximum length for environment variable name and value
    static constexpr int MaxSystemEnvVariableLength = 1024;
    static constexpr int MaxUserEnvVariableLength = 255;
    static constexpr int MaxMachineNameLength = 256;

    static OperatingSystem m_OS;
    static bool m_IsWindows8OrAbove;
    static bool m_CheckedOSWin8OrAbove;

    static void _Exit(int exitCode);

public:

    static OperatingSystem OSVersion();
    static bool IsWindows8OrAbove();
    static void Exit(int exitCode);
    std::string CurrentDirectory();
    std::string SystemDirectory();
    std::string InternalWindowsDirectory();
    std::string MachineName();
    bool Is64BitProcess();
    bool Is64OperatingSystem();
};