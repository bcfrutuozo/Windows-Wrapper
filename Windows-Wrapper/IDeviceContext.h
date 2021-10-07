#pragma once

#include "IntPtr.h"
#include "IDisposable.h"

class IDeviceContext : public IDisposable
{
private:

	IntPtr m_HDC;

protected:

	inline void SetHDC(IntPtr dcHandle) noexcept
	{ 
		m_HDC = dcHandle; 
	}

public:

	inline constexpr IntPtr GetHDC() const noexcept { return m_HDC; }
};