#pragma once

template<typename T>
class IComparable
{
public:

	virtual int CompareTo(Object* const b) const = 0;
	virtual int CompareTo(const T& b) const noexcept = 0;
};