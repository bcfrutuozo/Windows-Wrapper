#pragma once

#include "Type.h"
#include "uuid.h"
#include "Event.h"

#include <string>

class Base : public IEquatable<Base>
{
private:

	uuids::uuid m_UUID;

protected:

	Base();
	virtual ~Base();

public:

	bool Equals(const Base& b) const noexcept;
	const std::string& GetHashCode() const noexcept;
	const Type GetType() const noexcept;
	const bool ReferenceEquals(const Base& b) const noexcept;
	virtual const std::string ToString() const noexcept;
};