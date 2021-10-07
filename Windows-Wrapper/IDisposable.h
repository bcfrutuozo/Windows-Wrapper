#pragma once

#include "CommonObject.h"

class IDisposable : public Object
{
protected:

	bool Disposing = false;
	bool Disposed = false;

public:

	inline constexpr bool IsDisposing() const noexcept { return Disposing; }
	inline constexpr bool IsDisposed() const noexcept { return Disposed; }
	virtual void Dispose() = 0;
};