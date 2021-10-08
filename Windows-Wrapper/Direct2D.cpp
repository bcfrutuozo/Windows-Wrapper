#include "Direct2D.h"
#include "WinAPI.h"
#include "Exceptions.h"

ID2D1Factory* Direct2D::m_pDirect2dFactory = nullptr;
IDWriteFactory* Direct2D::m_pDWriteFactory = nullptr;

ID2D1Factory* Direct2D::CreateDeviceIndependentResources()
{
	// Create a Direct2D factory.
	ID2D1Factory* d2dFactory;

	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	if (!SUCCEEDED(hr)) throw ExternalException("Error creating D2D device", hr);

	// Create a DirectWrite factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

	if (!SUCCEEDED(hr)) throw ExternalException("Error creating DirectWrite object", hr);

	return d2dFactory;
}

void Direct2D::DiscardDeviceResources()
{
	SafeRelease(&m_pDirect2dFactory);
}

void Direct2D::SetRenderTarget()
{
	HWND hwnd = static_cast<HWND>(WindowHandle.ToPointer());

	D2D1_SIZE_U size = D2D1::SizeU(WindowSize.Width, WindowSize.Height);

	// Create a Direct2D render target.
	auto hr = m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget
	);

	// Set DCHandl to D2D RenderTarget
	DCHandle = m_pDirect2dFactory;

	if (!SUCCEEDED(hr)) throw ExternalException("Error setting render target", hr);
}

void Direct2D::Initialize()
{
	SetRenderTarget();
}


void Direct2D::BeginDraw()
{
	m_pRenderTarget->BeginDraw();
}

Direct2D::Direct2D(IntPtr windowHandle, Size size)
	:
	Graphics(windowHandle, size)
{
	Initialize();
}

void Direct2D::EndDraw()
{
	auto hr = m_pRenderTarget->EndDraw();

	if (!SUCCEEDED(hr)) throw ExternalException("Could not finish WM_PAINT event", hr);
}

void Direct2D::ClearResources()
{
	for (auto p : Elements)
	{
		auto obj = (IUnknown*)p.second;
		SafeRelease(&obj);
	}

	SafeRelease(&m_pRenderTarget);
}

const IntPtr Direct2D::CreateSolidBrush(Color c)
{
	ID2D1SolidColorBrush* brush = nullptr;

	if (Elements.contains(c.ToString()))
	{
		brush = (ID2D1SolidColorBrush*)GetElement(c.ToString());

		if (!brush) throw ArgumentException("Element already exist but it's not from ID2D1SolidColorBrush type");

		return brush;
	}

	auto hr = m_pRenderTarget->CreateSolidColorBrush(c.ToD3DColor(), &brush);

	if (!SUCCEEDED(hr)) throw ExternalException("Could not create solid color brush", hr);

	Elements.insert(std::pair<std::string, void*>(c.ToString(), brush));

	return brush;
}

const IntPtr Direct2D::CreateFontObject(const Font& f)
{
	HFONT font = { 0 };

	if (Elements.contains(f.ToString()))
	{
		font = (HFONT)GetElement(f.ToString());

		if (!font) throw ArgumentException("Element already exist but it's not from HFONT type");

		return font;
	}

	//m_pRenderTarget->CreatFon
	//
	//if (!font) throw ExternalException("Could not create font", GetLastError());
	//
	//Elements.insert(std::pair<std::string, void*>(f.ToString(), font));

	return font;
}

Drawing::Rectangle Direct2D::DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle)
{
	ID2D1Brush* brush = (ID2D1Brush*)GetElement(c.ToString());

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	D2D1_RECT_F a;
	a.left = 0.0f;
	a.top = 0.0f;
	a.right = (float)rect.Width;
	a.bottom = (float)rect.Height;

	m_pRenderTarget->DrawRectangle(a, brush);

	return Drawing::Rectangle::Inflate(rect, -borderSize, -borderSize);
}

Drawing::Rectangle Direct2D::DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius)
{
	throw NotImplementedException();
}

void Direct2D::FillRectangle(Color c, Drawing::Rectangle rect)
{
	ID2D1Brush* brush = (ID2D1Brush*)GetElement(c.ToString());

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	D2D1_RECT_F a;
	a.left = 0.0f;
	a.top = 0.0f;
	a.right = (float)rect.Width;
	a.bottom = (float)rect.Height;

	m_pRenderTarget->FillRectangle(a, brush);
}

void Direct2D::FillRoundedRectangle(Color c, Drawing::Rectangle rect, int radius)
{
	ID2D1Brush* brush = (ID2D1Brush*)GetElement(c.ToString());

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	D2D1_RECT_F a;
	a.left = 0.0f;
	a.top = 0.0f;
	a.right = (float)rect.Width;
	a.bottom = (float)rect.Height;

	D2D1_ROUNDED_RECT rr;
	rr.radiusX = (float)radius;
	rr.radiusY = (float)radius;

	m_pRenderTarget->FillRoundedRectangle(rr, brush);
}

void Direct2D::CommonDrawText(const std::string& text, const Font& font, Color c, Drawing::Rectangle rect)
{
	//// Create a DirectWrite text format object.
	//auto hr = m_pDWriteFactory->CreateTextFormat(
	//	font.GetName().c_str(),
	//	NULL,
	//	DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL,
	//	DWRITE_FONT_STRETCH_NORMAL,
	//	static_cast<float>(font.GetSize()),
	//	L"", //locale
	//	&m_pTextFormat
	//);
	//
	//D2D1_RECT_F rect;
	//
	//
	//m_pRenderTarget->DrawTextA(, , m_pTextFormat, rect, brush);
}
