#pragma once

#include "Object.h"

#include <Windows.h>

enum class PowerLineStatus
{
	Offline = 0,
	Online = 1,
	Unknown = 255
};

enum class BatteryChargeStatus
{
	High = 1,
	Low = 2,
	Critical = 4,
	Charging = 8,
	NoSystemBattery = 128,
	Unknown = 255
};

enum class PowerState
{
	Suspend = 0,
	Hibernate = 1
};

class PowerStatus : public Object
{
	friend class SystemInformation;

private:

	SYSTEM_POWER_STATUS m_SystemPowerStatus;

	PowerStatus();

	void UpdateSystemPowerStatus() noexcept;

public:

	BatteryChargeStatus GetBatteryChargeStatus();
	int GetBatteryFullLifeTime();
	float GetBatteryLifePercent();
	int GetBatteryLifeRemaining();
	PowerLineStatus GetPowerLineStatus();
};

