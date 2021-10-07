#pragma once

#include "Graphics.h"

class WinAPI;

class Direct2D final : public Graphics
{
	friend class Graphics;

private:

	static ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	static ID2D1Factory* CreateDeviceIndependentResources();

	Direct2D(IntPtr windowHandle);

	void Initialize();
	void SetRenderTarget();

protected:

public:

	virtual ~Direct2D();

	void BeginDraw() override;
	void EndDraw() override;
	void ReleaseHDC() noexcept override;

	const IntPtr CreateSolidBrush(const std::string& name, Color c) override;
	void FillRectangle(Drawing::Rectangle rect, const std::string& brushName) override;
	void FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName) override;
};