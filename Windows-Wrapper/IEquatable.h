#pragma once

template<class T>
class IEquatable
{
	virtual bool Equals(const T&) const noexcept = 0;
};