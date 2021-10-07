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

	const IntPtr CreateSolidBrush(const std::string& name, Color c) override;
	void FillRectangle(Drawing::Rectangle rect, const std::string& brushName) override;
	void FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName) override;
};