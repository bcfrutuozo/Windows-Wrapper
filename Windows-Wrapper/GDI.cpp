#include "GDI.h"
#include "Exceptions.h"

GDI::GDI(IntPtr window)
	:
	Graphics(window),
	ps({ 0 })
{

}

GDI::~GDI()
{
}

void GDI::BeginDraw()
{
	BeginPaint(static_cast<HWND>(GetWindowHandle()), &ps);
	SetHDC(ps.hdc);
}

void GDI::EndDraw()
{
	EndPaint(static_cast<HWND>(GetWindowHandle()), &ps);
}

void GDI::ReleaseHDC() noexcept
{
	//ReleaseDC(static_cast<HWND>(GetWindowHandle()), static_cast<HDC>(GetHDC().ToPointer()));
}

const IntPtr GDI::CreateSolidBrush(const std::string& name, Color c)
{
	HBRUSH brush;

	if (Elements->contains(name))
	{
		brush = (HBRUSH)Elements->at(name);

		if (!brush) throw ArgumentException("Element already exist but it's not from ID2D1SolidColorBrush type");

		return brush;
	}

	brush = ::CreateSolidBrush(c.ToRGB());

	if (!brush) throw ExternalException("Could not create solid color brush", GetLastError());

	Elements->insert(std::pair<std::string, void*>(name, brush));

	return brush;
}

void GDI::FillRectangle(Drawing::Rectangle rect, const std::string& brushName)
{
	if (!Elements->contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	HBRUSH brush = (HBRUSH)Elements->at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = rect.Width;
	rc.bottom = rect.Height;

	::FillRect(static_cast<HDC>(GetHDC().ToPointer()), &rc, brush);
}

void GDI::FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName)
{
	if (!Elements->contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	HBRUSH brush = (HBRUSH)Elements->at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = rect.Width;
	rc.bottom = rect.Height;

	SelectObject(static_cast<HDC>(GetHDC().ToPointer()), brush);
	::RoundRect(static_cast<HDC>(GetHDC().ToPointer()), rc.left, rc.top, rc.right, rc.bottom, width, height);
}
