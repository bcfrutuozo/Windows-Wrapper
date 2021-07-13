#pragma once

template<class T>
class IEquatable
{
	virtual bool Equals(T) const noexcept = 0;
};