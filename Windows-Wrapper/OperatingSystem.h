#pragma once

#include "Object.h"
#include "Version.h"
#include "PlatformID.h"

#include <string>

class OperatingSystem : public Object
{
	friend class Environment;

private:

	bool Loaded = false;
	Version m_Version;
	PlatformID m_Platform;
	std::string m_ServicePack;
	mutable std::string m_VersionString;

	OperatingSystem();
	OperatingSystem(PlatformID platform, Version version, const std::string& servicePack);

public:

	OperatingSystem(PlatformID platform, Version version);

	PlatformID GetPlatform() const noexcept;
	std::string GetServicePack() const noexcept;
	Version GetVersion() const noexcept;
	std::string ToString() const noexcept override;
	std::string VersionString() const noexcept;
};

