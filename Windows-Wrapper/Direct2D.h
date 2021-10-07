#pragma once

#include "Graphics.h"

class WinAPI;

class Direct2D final : public Graphics
{
	friend class Graphics;
	friend class Application;

private:

	static ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	Direct2D(IntPtr windowHandle, Size size);

	void Initialize();
	void SetRenderTarget();
	void ClearResources() override;

	static ID2D1Factory* CreateDeviceIndependentResources();
	static void DiscardDeviceResources();

public:

	void BeginDraw() override;
	void EndDraw() override;

	const IntPtr CreateSolidBrush(const std::string& name, Color c) override;
	void FillRectangle(Drawing::Rectangle rect, const std::string& brushName) override;
	void FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName) override;
};