#pragma once

template<class T>
class IEquatable
{
	virtual bool operator==(const T& b) const noexcept = 0;
	virtual bool Equals(const T&) const noexcept = 0;
};