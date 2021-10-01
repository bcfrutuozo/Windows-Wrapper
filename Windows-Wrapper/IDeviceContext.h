#pragma once

#include "IntPtr.h"
#include "IDisposable.h"

class IDeviceContext : public IDisposable
{
private:

	IntPtr m_HDC;

public:

	constexpr IDeviceContext(IntPtr hdcHandle) : m_HDC(hdcHandle) { }

	inline constexpr IntPtr GetHDC() const noexcept { return m_HDC; }
};