#pragma once

#include "Type.h"
#include "uuid.h"

#include <string>

class Base : public IEquatable<Base>
{
private:

	uuids::uuid m_UUID;

protected:

	Base();
	virtual ~Base();

public:

	inline bool operator==(const Base& b) const noexcept;
	inline bool static Equals(const Base& lhs, const Base& rhs) noexcept;
	inline virtual bool Equals(const Base& b) const noexcept;
	inline const std::string& GetHashCode() const noexcept;
	inline const Type GetType() const noexcept;
	inline const bool ReferenceEquals(const Base& b) const noexcept;
	inline virtual const std::string ToString() const noexcept;
};