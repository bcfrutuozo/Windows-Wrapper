#include "Direct2D.h"
#include "WinAPI.h"
#include "Exceptions.h"

ID2D1Factory* Direct2D::m_pDirect2dFactory = nullptr;

ID2D1Factory* Direct2D::CreateDeviceIndependentResources()
{
	// Create a Direct2D factory.
	ID2D1Factory* d2dFactory;
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	if (!SUCCEEDED(hr)) throw ExternalException("Error creating D2D device", hr);
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
	if (!m_pDirect2dFactory) m_pDirect2dFactory = CreateDeviceIndependentResources();
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

const IntPtr Direct2D::CreateSolidBrush(const std::string& name, Color c)
{
	ID2D1SolidColorBrush* brush = nullptr;

	if (Elements.contains(name))
	{
		brush = (ID2D1SolidColorBrush*)Elements.at(name);

		if (!brush) throw ArgumentException("Element already exist but it's not from ID2D1SolidColorBrush type");

		return brush;
	}

	auto hr = m_pRenderTarget->CreateSolidColorBrush(c.ToD3DColor(), &brush);

	if (!SUCCEEDED(hr)) throw ExternalException("Could not create solid color brush", hr);

	Elements.insert(std::pair<std::string, void*>(name, brush));

	return brush;
}

void Direct2D::FillRectangle(Drawing::Rectangle rect, const std::string& brushName)
{
	if (!Elements.contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	ID2D1Brush* brush = (ID2D1Brush*)Elements.at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	D2D1_RECT_F a;
	a.left = 0.0f;
	a.top = 0.0f;
	a.right = (float)rect.Width;
	a.bottom = (float)rect.Height;

	m_pRenderTarget->FillRectangle(a, brush);
}

void Direct2D::FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName)
{
	if (!Elements.contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	ID2D1Brush* brush = (ID2D1Brush*)Elements.at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	D2D1_RECT_F a;
	a.left = 0.0f;
	a.top = 0.0f;
	a.right = (float)rect.Width;
	a.bottom = (float)rect.Height;

	D2D1_ROUNDED_RECT rr;
	rr.radiusX = (float)width;
	rr.radiusY = (float)height;

	m_pRenderTarget->FillRoundedRectangle(rr, brush);
}
