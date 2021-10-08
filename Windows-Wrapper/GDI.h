#pragma once

#include "Graphics.h"

class GDI final : public Graphics
{
	friend class Graphics;

private:

	PAINTSTRUCT m_PS;
	HDC m_MemoryDC;
	HBITMAP m_MemoryBitmap;
	HBITMAP m_OldBitmap;

	GDI(IntPtr window, Size size);

public:

	void ClearResources() override;
	void BeginDraw() override;
	void EndDraw() override;

	const IntPtr CreateSolidBrush(Color c) override;
	const IntPtr CreateFontObject(const Font& f) override;
	Drawing::Rectangle DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle) override;
	Drawing::Rectangle DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius) override;
	void FillRectangle(Color c, Drawing::Rectangle rect) override;
	void FillRoundedRectangle(Color c, Drawing::Rectangle rect,int radius) override;
	void CommonDrawText(const std::string& text, const Font& font, Color c, Drawing::Rectangle rect) override;
};