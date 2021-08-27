#pragma once

#include "Type.h"
#include "uuid.h"

#include <string>

class Object : public IEquatable<Object>
{
private:

	uuids::uuid m_UUID;

public:

	Object() noexcept;
	Object(const Object&) = default;			// Copy constructor
	Object(Object&&) = default;					// Move constructor
	Object& operator=(const Object&) = default;	// Copy assignment constructor
	Object& operator=(Object&&) = default;		// Move assignment constructor
	virtual ~Object() = default;				// Destructor

	virtual bool operator==(const Object& b) const noexcept override;
	virtual bool Equals(const Object& b) const noexcept override;

	bool static Equals(const Object& lhs, const Object& rhs) noexcept;
	inline const std::string& GetHashCode() const noexcept;
	inline const Type GetType() const noexcept;
	inline const bool ReferenceEquals(const Object& b) const noexcept;
	inline virtual const std::string ToString() const noexcept;
};