#pragma once

#include "Graphics.h"

class WinAPI;

class Direct2D final : public Graphics
{
	friend class Graphics;
	friend class Application;

private:

	static ID2D1Factory* m_pDirect2dFactory;
	static IDWriteFactory* m_pDWriteFactory;

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

	const IntPtr CreateSolidBrush(Color c) override;
	const IntPtr CreateFontObject(const Font& f) override;
	Drawing::Rectangle DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle) override;
	Drawing::Rectangle DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius) override;
	void FillRectangle(Color c, Drawing::Rectangle rect) override;
	void FillRoundedRectangle(Color c, Drawing::Rectangle rect, int radius) override;
	void CommonDrawText(const std::string& text, const Font& font, Color c, Drawing::Rectangle rect) override;
};