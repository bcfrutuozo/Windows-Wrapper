#pragma once

#include "Object.h"
#include "IComparable.h"
#include "Exceptions.h"

template<typename W, typename T>
class ValueType : public Object, public IComparable<T>, public IEquatable<T>
{
protected:

	T ValueObject;

	constexpr ValueType() : ValueObject() {}
	constexpr ValueType(T v) : ValueObject(v) {}

public:

	operator T() const { return ValueObject; }

	// Modifiers
	W& operator=(T v) noexcept { ValueObject = v; return *this; }
	W& operator+=(T v) noexcept { ValueObject += v; return *this; }
	W& operator-=(T v) noexcept { ValueObject -= v; return *this; }
	W& operator*=(T v) noexcept { ValueObject *= ValueObject; return *this; }
	W& operator/=(T v) noexcept { ValueObject /= ValueObject; return *this; }
	W& operator%=(T v) noexcept { ValueObject %= ValueObject; return *this; }
	W& operator++() noexcept { ++ValueObject; return *this; }
	W& operator--() noexcept { --ValueObject; return *this; }
	W operator++(int) const noexcept { return W(ValueObject++); }
	W operator--(int) const noexcept { return W(ValueObject--); }
	W& operator&=(T v) noexcept { ValueObject &= v; return *this; }
	W& operator|=(T v) noexcept { ValueObject |= v; return *this; }
	W& operator^=(T v) noexcept { ValueObject ^= v; return *this; }
	W& operator<<=(T v) noexcept { ValueObject <<= v; return *this; }
	W& operator>>=(T v) noexcept { ValueObject >>= v; return *this; }

	// Accessors
	W operator+() const noexcept { return W(+ValueObject); }
	W operator-() const noexcept { return W(-ValueObject); }
	W operator!() const noexcept { return W(!ValueObject); }
	W operator~() const noexcept { return W(~ValueObject); }

	// Friends
	inline friend W operator+(W iw, W v) noexcept { return iw += v; }
	inline friend W operator+(W iw, T v) noexcept { return iw += v; }
	inline friend W operator+(T v, W iw) noexcept { return W(v) += iw; }
	inline friend W operator-(W iw, W v) noexcept { return iw -= v; }
	inline friend W operator-(W iw, T v) noexcept { return iw -= v; }
	inline friend W operator-(T v, W iw) noexcept { return W(v) -= iw; }
	inline friend W operator*(W iw, W v) noexcept { return iw *= v; }
	inline friend W operator*(W iw, T v) noexcept { return iw *= v; }
	inline friend W operator*(T v, W iw) noexcept { return W(v) *= iw; }
	inline friend W operator/(W iw, W v) noexcept { return iw /= v; }
	inline friend W operator/(W iw, T v) noexcept { return iw /= v; }
	inline friend W operator/(T v, W iw) noexcept { return W(v) /= iw; }
	inline friend W operator%(W iw, W v) noexcept { return iw %= v; }
	inline friend W operator%(W iw, T v) noexcept { return iw %= v; }
	inline friend W operator%(T v, W iw) noexcept { return W(v) %= iw; }
	inline friend W operator&(W iw, W v) noexcept { return iw &= v; }
	inline friend W operator&(W iw, T v) noexcept { return iw &= v; }
	inline friend W operator&(T v, W iw) noexcept { return W(v) &= iw; }
	inline friend W operator|(W iw, W v) noexcept { return iw |= v; }
	inline friend W operator|(W iw, T v) noexcept { return iw |= v; }
	inline friend W operator|(T v, W iw) noexcept { return W(v) |= iw; }
	inline friend W operator^(W iw, W v) noexcept { return iw ^= v; }
	inline friend W operator^(W iw, T v) noexcept { return iw ^= v; }
	inline friend W operator^(T v, W iw) noexcept { return W(v) ^= iw; }
	inline friend W operator<<(W iw, W v) noexcept { return iw <<= v; }
	inline friend W operator<<(W iw, T v) noexcept { return iw <<= v; }
	inline friend W operator<<(T v, W iw) noexcept { return W(v) <<= iw; }
	inline friend W operator>>(W iw, W v) noexcept { return iw >>= v; }
	inline friend W operator>>(W iw, T v) noexcept { return iw >>= v; }
	inline friend W operator>>(T v, W iw) noexcept { return W(v) >>= iw; }

	int CompareTo(Object* const b) const override
	{
		if (b == nullptr)
		{
			return 1;
		}

		if (GetType() == b->GetType())
		{
			T* i2 = reinterpret_cast<T*>(b);
			T i = ValueObject;
			if (*i2 < i) return -1;
			if (*i2 > i) return 1;

			return 0;
		}

		std::ostringstream oss;
		oss << "Arg_MustBe" << GetType().ToString() << std::endl;
		throw ArgumentException(oss.str().c_str());
	}

	int CompareTo(const T& b) const noexcept override
	{
		if (ValueObject < b) return -1;
		if (ValueObject > b) return 1;

		return 0;
	}

	bool operator==(const T& b) const noexcept
	{
		return ValueObject == b.ValueObject;
	}

	bool Equals(const T& b) const noexcept
	{
		return ValueObject == b.ValueObject;
	}
};