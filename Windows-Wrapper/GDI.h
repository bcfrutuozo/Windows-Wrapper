#pragma once

#include "Graphics.h"

class GDI final : public Graphics
{
	friend class Graphics;

private:

	PAINTSTRUCT ps;

	GDI(IntPtr window);
	virtual ~GDI();

public:

	void BeginDraw() override;
	void EndDraw() override;
	void ReleaseHDC() noexcept override;

	const IntPtr CreateSolidBrush(const std::string& name, Color c) override;
	void FillRectangle(Drawing::Rectangle rect, const std::string& brushName) override;
	void FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName) override;
};