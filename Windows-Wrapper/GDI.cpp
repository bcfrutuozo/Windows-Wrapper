#include "GDI.h"
#include "Exceptions.h"

GDI::GDI(IntPtr window, Size size)
	:
	Graphics(window, size),
	m_PS({ 0 }),
	m_MemoryBitmap(nullptr),
	m_MemoryDC(nullptr),
	m_OldBitmap(nullptr)
{

}

void GDI::ClearResources()
{
	for (auto p : Elements)
	{
		auto obj = (HGDIOBJ)&p;
		SelectObject(static_cast<HDC>(GetHDC().ToPointer()), obj);
		DeleteObject(obj);
	}
}

void GDI::BeginDraw()
{
	DCHandle = BeginPaint(static_cast<HWND>(WindowHandle.ToPointer()), &m_PS);
	m_MemoryDC = CreateCompatibleDC(static_cast<HDC>(DCHandle.ToPointer()));
	m_MemoryBitmap = CreateCompatibleBitmap(static_cast<HDC>(DCHandle.ToPointer()), WindowSize.Width, WindowSize.Height);
	m_OldBitmap = (HBITMAP)SelectObject(m_MemoryDC, m_MemoryBitmap);
}

void GDI::EndDraw()
{
	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(static_cast<HDC>(DCHandle.ToPointer()), 0, 0, WindowSize.Width, WindowSize.Height, m_MemoryDC, 0, 0, SRCCOPY);
	SelectObject(m_MemoryDC, m_OldBitmap);
	ReleaseDC(static_cast<HWND>(WindowHandle.ToPointer()), m_MemoryDC);
	DeleteDC(m_MemoryDC);

	EndPaint(static_cast<HWND>(WindowHandle.ToPointer()), &m_PS);
}

const IntPtr GDI::CreateSolidBrush(const std::string& name, Color c)
{
	HBRUSH brush;

	if (Elements.contains(name))
	{
		brush = (HBRUSH)Elements.at(name);

		if (!brush) throw ArgumentException("Element already exist but it's not from ID2D1SolidColorBrush type");

		return brush;
	}

	brush = ::CreateSolidBrush(c.ToRGB());

	if (!brush) throw ExternalException("Could not create solid color brush", GetLastError());

	Elements.insert(std::pair<std::string, void*>(name, brush));

	return brush;
}

void GDI::FillRectangle(Drawing::Rectangle rect, const std::string& brushName)
{
	if (!Elements.contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	HBRUSH brush = (HBRUSH)Elements.at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = rect.Width;
	rc.bottom = rect.Height;

	::FillRect(m_MemoryDC, &rc, brush);
}

void GDI::FillRoundedRectangle(Drawing::Rectangle rect, int width, int height, const std::string& brushName)
{
	if (!Elements.contains(brushName)) throw ArgumentNullException("brushName doesn't exist");

	HBRUSH brush = (HBRUSH)Elements.at(brushName);

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = rect.Width;
	rc.bottom = rect.Height;

	SelectObject(m_MemoryDC, brush);
	::RoundRect(m_MemoryDC, rc.left, rc.top, rc.right, rc.bottom, width, height);
}
