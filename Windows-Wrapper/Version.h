#pragma once

#include "IComparable.h"
#include "IEquatable.h"

#include <string>
#include <sstream>

class Version final : public IComparable<Version>, public IEquatable<Version>
{
	friend class OperatingSystem;

private:

	int m_Major;
	int m_Minor;
	int m_Build;
	int m_Revision;

	static constexpr int ZERO_CHAR_VALUE = (int)'0';
	static constexpr char SeparatorsArray = '.';

	Version();

	static void AppendPositiveNumber(int num, std::ostringstream& oss);
	static void ParseComponent(const std::string& component, const std::string& componentName, int& value);

public:

	Version(int major, int minor, int build, int revision);
	Version(int major, int minor, int build);
	Version(int major, int minor);
	Version(const std::string& version);

	bool operator==(Version v);
	bool operator!=(Version v);
	bool operator<(Version v);
	bool operator<=(Version v);
	bool operator>(Version v);
	bool operator>=(Version v);

	int CompareTo(const Object* const b) const override;
	int CompareTo(const Version* const b) const override;
	bool Equals(const Object* const b) const override;
	bool Equals(const Version* const b) const override;
	int GetBuild() const noexcept;
	int GetHashCode() const noexcept override;
	int GetMajor() const noexcept;
	short GetMajorRevision() const noexcept;
	int GetMinor() const noexcept;
	short GetMinorRevision() const noexcept;
	int GetRevision() const noexcept;
	static Version Parse(const std::string& input);
	std::string ToString() const noexcept override;
	std::string ToString(int fieldCount) const noexcept;
};

