#pragma once

#include "IntPtr.h"
#include "Color.h"
#include "Rectangle.h"
#include "IDisposable.h"
#include "Font.h"

class Graphics : public IDisposable
{

private:

	

public:

	IntPtr DCHandle;
	IntPtr WindowHandle;
	Size WindowSize;
	std::map<std::string, void*> Elements;

	Graphics(IntPtr window, Size size);
	virtual ~Graphics() { }
	IntPtr GetHDC() const noexcept { return DCHandle; }

	static Graphics* Create(IntPtr window, Size size);

	void UpdateSize(Size size) { WindowSize = size; }

	void* GetElement(const std::string& name) const
	{
		for (auto& p : Elements)
		{
			if (p.first == name)
				return p.second;
		}

		return nullptr;
	}

	void Dispose() override
	{
		DeleteDC((HDC)DCHandle.ToPointer());
	}

	virtual void ClearResources() = 0;
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;

	virtual const IntPtr CreateSolidBrush(Color c) = 0;
	virtual const IntPtr CreateFontObject(const Font& f) = 0;
	virtual Drawing::Rectangle DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle) = 0;
	virtual Drawing::Rectangle DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius) = 0;
	virtual void FillRectangle(Color c, Drawing::Rectangle rect) = 0;
	virtual void FillRoundedRectangle(Color c, Drawing::Rectangle rect, int radius) = 0;
	virtual void CommonDrawText(const std::string& text, const Font& font, Color c, Drawing::Rectangle rect) = 0;
};

