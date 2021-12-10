#include "PowerStatus.h"

PowerStatus::PowerStatus()
	:
	m_SystemPowerStatus({0})
{}

void PowerStatus::UpdateSystemPowerStatus() noexcept
{
	GetSystemPowerStatus(&m_SystemPowerStatus);
}

BatteryChargeStatus PowerStatus::GetBatteryChargeStatus()
{
	UpdateSystemPowerStatus();
	return (BatteryChargeStatus)m_SystemPowerStatus.BatteryFlag;
}

int PowerStatus::GetBatteryFullLifeTime()
{
	UpdateSystemPowerStatus();
	return m_SystemPowerStatus.BatteryFullLifeTime;
}

float PowerStatus::GetBatteryLifePercent()
{
	UpdateSystemPowerStatus();
	float lifePercent = m_SystemPowerStatus.BatteryLifePercent / 100.0f;
	return lifePercent > 1.0f ? 1.0f : lifePercent;
}

int PowerStatus::GetBatteryLifeRemaining()
{
	UpdateSystemPowerStatus();
	return m_SystemPowerStatus.BatteryLifeTime;
}

PowerLineStatus PowerStatus::GetPowerLineStatus()
{
	UpdateSystemPowerStatus();
	return (PowerLineStatus)m_SystemPowerStatus.ACLineStatus;
}
