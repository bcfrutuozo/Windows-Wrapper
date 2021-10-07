#pragma once

#include "IntPtr.h"
#include "IDisposable.h"

class IDeviceContext : public IDisposable
{
protected:

	IntPtr DCHandle;

public:

	virtual void Dispose() override = 0;

	inline constexpr IntPtr GetHDC() const { return DCHandle; };
};