#pragma once

#include "Type.h"
#include "uuid.h"

#include <string>

class Base : public IEquatable<Base>
{
private:

	uuids::uuid m_UUID;

public:

	Base();
	Base(const Base&) = default;			// Copy constructor
	Base(Base&&) = default;					// Move constructor
	Base& operator=(const Base&) = default;	// Copy assignment constructor
	Base& operator=(Base&&) = default;		// Move assignment constructor
	virtual ~Base() = default;				// Destructor

	inline bool operator==(const Base& b) const noexcept;
	inline bool static Equals(const Base& lhs, const Base& rhs) noexcept;
	inline virtual bool Equals(const Base& b) const noexcept;
	inline const std::string& GetHashCode() const noexcept;
	inline const Type GetType() const noexcept;
	inline const bool ReferenceEquals(const Base& b) const noexcept;
	inline virtual const std::string ToString() const noexcept;
};