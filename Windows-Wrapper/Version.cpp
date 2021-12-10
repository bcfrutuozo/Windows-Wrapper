#include "Version.h"
#include "Exceptions.h"

#include <vector>
#include <cassert>

void Version::AppendPositiveNumber(int num, std::ostringstream& oss)
{
	assert(num >= 0, "AppendPositiveNumber expect positive numbers");

	int reminder = 0;

	do
	{
		reminder = num % 10;
		num = num / 10;
		oss << (char)(ZERO_CHAR_VALUE + reminder);
	} while(num > 0);
}

void Version::ParseComponent(const std::string& component, const std::string& componentName, int& value)
{
	try
	{
		value = stoi(component);
	}
	catch(...)
	{
		throw FormatException("Could not parse Major component");
	}

	if(value < 0) throw ArgumentOutOfRangeException("Major value is negative");
}

Version::Version(int major, int minor, int build, int revision)
{
	if(major < 0) throw ArgumentOutOfRangeException("major", "ArgumentOutOfRange_Version");
	if(minor < 0) throw ArgumentOutOfRangeException("minor", "ArgumentOutOfRange_Version");
	if(build < 0) throw ArgumentOutOfRangeException("build", "ArgumentOutOfRange_Version");
	if(revision < 0) throw ArgumentOutOfRangeException("revision", "ArgumentOutOfRange_Version");

	m_Major = major;
	m_Minor = minor;
	m_Build = build;
	m_Revision = revision;
}

Version::Version()
	:
	m_Major(0),
	m_Minor(0),
	m_Build(-1),
	m_Revision(-1)
{}

Version::Version(int major, int minor, int build)
	:
	m_Revision(-1)
{
	if(major < 0) throw ArgumentOutOfRangeException("major", "ArgumentOutOfRange_Version");
	if(minor < 0) throw ArgumentOutOfRangeException("minor", "ArgumentOutOfRange_Version");
	if(build < 0) throw ArgumentOutOfRangeException("build", "ArgumentOutOfRange_Version");

	m_Major = major;
	m_Minor = minor;
	m_Build = build;
}

Version::Version(int major, int minor)
	:
	m_Build(-1),
	m_Revision(-1)
{
	if(major < 0) throw ArgumentOutOfRangeException("major", "ArgumentOutOfRange_Version");
	if(minor < 0) throw ArgumentOutOfRangeException("minor", "ArgumentOutOfRange_Version");

	m_Major = major;
	m_Minor = minor;
}

bool Version::operator==(Version v)
{
	return Equals(&v);
}

bool Version::operator!=(Version v)
{
	return !(*this == v);
}

bool Version::operator<(Version v)
{
	return CompareTo(&v) < 0;
}

bool Version::operator<=(Version v)
{
	return CompareTo(&v) <= 0;
}

bool Version::operator>(Version v)
{
	return v.CompareTo(this) < 0;
}

bool Version::operator>=(Version v)
{
	return v.CompareTo(this) <= 0;
}

Version::Version(const std::string& version)
{
	auto v = Version::Parse(version);
	m_Major = v.m_Major;
	m_Minor = v.m_Minor;
	m_Build = v.m_Build;
	m_Revision = v.m_Revision;
}

int Version::CompareTo(const Object* const b) const
{
	if(b == nullptr) return 1;

	if(const Version* v = dynamic_cast<const Version*>(b))
	{
		if(m_Major != v->m_Major)
		{
			if(m_Major > v->m_Major) return 1;
			else return -1;
		}

		if(m_Minor != v->m_Minor)
		{
			if(m_Minor > v->m_Minor) return 1;
			else return -1;
		}

		if(m_Build != v->m_Build)
		{
			if(m_Build > v->m_Build) return 1;
			else return -1;
		}

		if(m_Revision != v->m_Revision)
		{
			if(m_Revision > v->m_Revision) return 1;
			else return -1;
		}

		return 0;
	}

	throw ArgumentException("MustBeVersion");
}

int Version::CompareTo(const Version* const b) const
{
	if(b == nullptr) return 1;

	if(m_Major != b->m_Major)
	{
		if(m_Major > b->m_Major) return 1;
		else return -1;
	}

	if(m_Minor != b->m_Minor)
	{
		if(m_Minor > b->m_Minor) return 1;
		else return -1;
	}

	if(m_Build != b->m_Build)
	{
		if(m_Build > b->m_Build) return 1;
		else return -1;
	}

	if(m_Revision != b->m_Revision)
	{
		if(m_Revision > b->m_Revision) return 1;
		else return -1;
	}

	return 0;
}

bool Version::Equals(const Object* const b) const
{
	if(b == nullptr) return false;

	if(const Version* v = dynamic_cast<const Version*>(b))
	{
		if(m_Major == v->m_Major && m_Minor == v->m_Minor && m_Build == v->m_Build && m_Revision == v->m_Revision) return true;
	}

	return false;
}

bool Version::Equals(const Version* const b) const
{
	if(b == nullptr) return false;

	if(m_Major == b->m_Major && m_Minor == b->m_Minor && m_Build == b->m_Build && m_Revision == b->m_Revision) return true;

	return false;
}

int Version::GetBuild() const noexcept
{
	return m_Build;
}

int Version::GetHashCode() const noexcept
{
	int accumulator = 0;

	accumulator |= (m_Major & 0x0000000F) << 28;
	accumulator |= (m_Minor & 0x000000FF) << 20;
	accumulator |= (m_Build & 0x000000FF) << 12;
	accumulator |= (m_Revision & 0x00000FFF);

	return accumulator;
}

int Version::GetMajor() const noexcept
{
	return m_Major;
}

short Version::GetMajorRevision() const noexcept
{
	return (short)(m_Revision >> 16);
}

int Version::GetMinor() const noexcept
{
	return m_Minor;
}

short Version::GetMinorRevision() const noexcept
{
	return (short)(m_Revision & 0xFFFF);
}

int Version::GetRevision() const noexcept
{
	return m_Revision;
}

Version Version::Parse(const std::string& input)
{
	if(input.empty()) throw ArgumentNullException("input");

	Version v;

	std::vector<std::string> tokens;
	std::istringstream f(input);
	std::string s;
	while(getline(f, s, SeparatorsArray))
	{
		tokens.push_back(s);
	}

	if(tokens.size() < 2 || tokens.size() > 4) throw ArgumentException("Arg_VersionString");

	int major = -1;
	int minor = -1;
	int build = -1;
	int revision = -1;
	ParseComponent(tokens[0], "Major", major);
	ParseComponent(tokens[1], "Minor", minor);
	if(tokens.size() > 2) ParseComponent(tokens[2], "Build", build);
	if(tokens.size() > 3) ParseComponent(tokens[3], "Revision", revision);

	if(build == -1 && revision == -1) return Version(major, minor);
	if(revision == -1) return Version(major, minor, build);
	return Version(major, minor, build, revision);
}

std::string Version::ToString() const noexcept
{
	if(m_Build == -1) return ToString(2);
	if(m_Revision == -1) return ToString(3);
	return ToString(4);
}

std::string Version::ToString(int fieldCount) const noexcept
{
	std::ostringstream oss;

	switch(fieldCount)
	{
		case 0: return std::string();
		case 1: return std::to_string(m_Major);
		case 2:
		{
			AppendPositiveNumber(m_Major, oss);
			oss << ".";
			AppendPositiveNumber(m_Minor, oss);
			return oss.str();
		}
		default:
		{
			if(m_Build == -1) throw ArgumentException("ArgumentOutOfRange_Bounds_Lower_Upper_0_2", "fieldCount");

			if(fieldCount == 3)
			{
				AppendPositiveNumber(m_Major, oss);
				oss << ".";
				AppendPositiveNumber(m_Minor, oss);
				oss << ".";
				AppendPositiveNumber(m_Build, oss);
				return oss.str();
			}

			if(m_Revision == -1) throw ArgumentException("ArgumentOutOfRange_Bounds_Lower_Upper_0_3", "fieldCount");

			if(fieldCount == 4)
			{
				AppendPositiveNumber(m_Major, oss);
				oss << ".";
				AppendPositiveNumber(m_Minor, oss);
				oss << ".";
				AppendPositiveNumber(m_Build, oss);
				oss << ".";
				AppendPositiveNumber(m_Revision, oss);
				return oss.str();
			}

			throw ArgumentException("ArgumentOutOfRange_Bounds_Lower_Upper_0_4", "fieldCount");
		}
	}
}
