#include "OperatingSystem.h"

#include <sstream>

OperatingSystem::OperatingSystem()
	:
	Loaded(false)
{}

OperatingSystem::OperatingSystem(PlatformID platform, Version version, const std::string& servicePack)
	:
	m_Platform(platform),
	m_Version(version),
	m_ServicePack(servicePack),
	Loaded(true)
{

}

OperatingSystem::OperatingSystem(PlatformID platform, Version version)
	:
	OperatingSystem(platform, version, "")
{

}

PlatformID OperatingSystem::GetPlatform() const noexcept
{
	return m_Platform;
}

std::string OperatingSystem::GetServicePack() const noexcept
{
	return m_ServicePack;
}

Version OperatingSystem::GetVersion() const noexcept
{
	return m_Version;
}

std::string OperatingSystem::ToString() const noexcept
{
	return VersionString();
}

std::string OperatingSystem::VersionString() const noexcept
{
	if(!m_VersionString.empty()) return m_VersionString;

	std::ostringstream oss;
	switch(m_Platform)
	{
		case PlatformID::Win32NT: oss << "Microsoft Windows NT"; break;
		case PlatformID::Win32Windows:
		{
			if((m_Version.GetMajor() > 4) || ((m_Version.GetMajor() == 4) && (m_Version.GetMinor() > 0))) oss << "Microsoft Windows 98";
			else "Microsoft Windows 95";
			
			break;
		}
		case PlatformID::Win32S: oss << "Microsoft WIN32S"; break;
		case PlatformID::WinCE: oss << "Microsoft Windows CE"; break;
		case PlatformID::MacOSX: oss << "Mac OS X"; break;
		default: oss << "<unknown> "; break;
	}
	
	if(m_ServicePack.empty()) oss << m_Version.ToString();
	else oss << m_Version.ToString(3) << " " << m_ServicePack;

	m_VersionString = oss.str();

	return m_VersionString;
}
