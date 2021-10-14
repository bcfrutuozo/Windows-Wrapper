#pragma once

#include "Graphics.h"
#include "Enums.h"

// Linker for Direct2D
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

// Direct2D
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// Template function to clear COM interfaces
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class NativeWindow;

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
	Size GetTextSize(const std::string& text, const Font& font) override;
	Size GetTextSize(const char* text, int length, const Font& font) override;
	Drawing::Rectangle DrawTransparentText(const std::string& text, HorizontalAlignment alignment, const Font& font, Color foreColor, Drawing::Rectangle rect, size_t cursorIndex = 0) override;
	Drawing::Rectangle DrawOpaqueText(const std::string& text, HorizontalAlignment alignment, const Font& font, Color foreColor, Drawing::Rectangle rect, Color backColor, size_t cursorIndex = 0) override;
};