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

const IntPtr GDI::CreateSolidBrush(Color c)
{
	HBRUSH brush;

	if (Elements.contains(c.ToString()))
	{
		brush = (HBRUSH)GetElement(c.ToString());

		if (!brush) throw ArgumentException("Element already exist but it's not from HBRUSH type");

		return brush;
	}

	brush = ::CreateSolidBrush(c.ToRGB());

	if (!brush) throw ExternalException("Could not create solid color brush", GetLastError());

	Elements.insert(std::pair<std::string, void*>(c.ToString(), brush));

	return brush;
}

const IntPtr GDI::CreateFontObject(const Font& f)
{
	HFONT font;

	if (Elements.contains(f.ToString()))
	{
		font = (HFONT)GetElement(f.ToString());

		if (!font) throw ArgumentException("Element already exist but it's not from HFONT type");

		return font;
	}

	font = ::CreateFont(
		f.GetSizeInPixels(),
		0,
		0,
		0,
		f.IsBold() ? FW_BOLD : FW_NORMAL,
		f.IsItalic(),
		f.IsUnderline(),
		f.IsStrikeOut(),
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		f.GetName().c_str());

	if (!font) throw ExternalException("Could not create font", GetLastError());

	Elements.insert(std::pair<std::string, void*>(f.ToString(), font));

	return font;
}

Drawing::Rectangle GDI::DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle)
{
	auto brush = Elements.contains(c.ToString()) ? Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = rect.GetLeft();
	rc.top = rect.GetTop();
	rc.right = rect.GetRight();
	rc.bottom = rect.GetBottom();

	int bordergdi = -1;

	switch (borderStyle)
	{
		case ChartDashStyle::Dash: bordergdi = PS_DASH; break;
		case ChartDashStyle::DashDot: bordergdi = PS_DASHDOT;  break;
		case ChartDashStyle::DashDotDot: bordergdi = PS_DASHDOTDOT; break;
		case ChartDashStyle::Dot: bordergdi = PS_DOT; break;
		case ChartDashStyle::Solid: bordergdi = PS_SOLID; break;
		default:
		throw ArgumentNullException("borderStyle");
	}

	HPEN pen = nullptr;
	HGDIOBJ old_pen = nullptr;

	SetBkMode(m_MemoryDC, OPAQUE);

	if (borderStyle != ChartDashStyle::Solid)
	{
		pen = CreatePen(bordergdi, 1, Color::Black().ToRGB());
		old_pen = SelectObject(m_MemoryDC, pen);
	}
	else
	{
		pen = CreatePen(PS_SOLID, borderSize, c.ToRGB());
		old_pen = SelectObject(m_MemoryDC, pen);	
	}

	SetBkColor(m_MemoryDC, c.ToRGB());
	SetBkMode(m_MemoryDC, OPAQUE);
	Rectangle(m_MemoryDC, rc.left, rc.top, rc.right, rc.bottom);

	//Clean up
	SelectObject(m_MemoryDC, old_pen);
	DeleteObject(old_pen);
	SelectObject(m_MemoryDC, pen);
	DeleteObject(pen);

	return Drawing::Rectangle::Deflate(rect, borderSize, borderSize);
}

Drawing::Rectangle GDI::DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius)
{
	auto brush = Elements.contains(c.ToString()) ? Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = rect.GetLeft();
	rc.top = rect.GetTop();
	rc.right = rect.GetRight();
	rc.bottom = rect.GetBottom();

	int bordergdi = -1;

	switch (borderStyle)
	{
		case ChartDashStyle::Dash: bordergdi = PS_DASH; break;
		case ChartDashStyle::DashDot: bordergdi = PS_DASHDOT;  break;
		case ChartDashStyle::DashDotDot: bordergdi = PS_DASHDOTDOT; break;
		case ChartDashStyle::Dot: bordergdi = PS_DOT; break;
		case ChartDashStyle::Solid: bordergdi = PS_SOLID; break;
		default:
		throw ArgumentNullException("borderStyle");
	}

	HPEN pen = nullptr;
	HGDIOBJ old_pen = nullptr;
	
	SetBkMode(m_MemoryDC, TRANSPARENT);
	
	if (borderStyle != ChartDashStyle::Solid)
	{
		pen = CreatePen(bordergdi, 0, Color::Black().ToRGB());
		old_pen = SelectObject(m_MemoryDC, pen);
	}
	else
	{
		pen = CreatePen(PS_SOLID, borderSize, c.ToRGB());
		old_pen = SelectObject(m_MemoryDC, pen);		
	}

	SetBkColor(m_MemoryDC, c.ToRGB());
	SetBkMode(m_MemoryDC, TRANSPARENT);
	RoundRect(m_MemoryDC, rc.left, rc.top, rc.right, rc.bottom, radius, radius);

	//Clean up
	SelectObject(m_MemoryDC, old_pen);
	DeleteObject(old_pen);
	SelectObject(m_MemoryDC, pen);
	DeleteObject(pen);

	return Drawing::Rectangle::Deflate(rect, borderSize, borderSize);
}

void GDI::FillRectangle(Color c, Drawing::Rectangle rect)
{
	auto brush = Elements.contains(c.ToString()) ? (HBRUSH)Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();

	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");

	RECT rc;
	rc.left = rect.GetLeft();
	rc.top = rect.GetTop();
	rc.right = rect.GetRight();
	rc.bottom = rect.GetBottom();

	FillRect(m_MemoryDC, &rc, brush);
}

void GDI::FillRoundedRectangle(Color c, Drawing::Rectangle rect, int radius)
{
	auto brush = Elements.contains(c.ToString()) ? (HBRUSH)Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();

	RECT rcLeftToRight;
	rcLeftToRight.left = rect.GetLeft();
	rcLeftToRight.top = rect.GetTop() + radius;
	rcLeftToRight.right = rect.GetRight();
	rcLeftToRight.bottom = rect.GetBottom() - radius;

	FillRect(m_MemoryDC, &rcLeftToRight, brush);

	RECT rcTopToBottom;
	rcTopToBottom.left = rect.GetLeft() + radius;
	rcTopToBottom.top = rect.GetTop();
	rcTopToBottom.right = rect.GetRight() - radius;
	rcTopToBottom.bottom = rect.GetBottom() ;

	FillRect(m_MemoryDC, &rcTopToBottom, brush);
}

void GDI::CommonDrawText(const std::string& text, const Font& font, Color c, Drawing::Rectangle rect)
{
	auto winFont = Elements.contains(font.ToString()) ? Elements.at(font.ToString()) : (HFONT)CreateFontObject(font).ToPointer();
	SelectObject(m_MemoryDC, winFont);

	SetBkMode(m_MemoryDC, TRANSPARENT);
	SetTextColor(m_MemoryDC, c.ToRGB());

	RECT rc;
	rc.left = rect.GetLeft();
	rc.top = rect.GetTop();
	rc.right = rect.GetRight();
	rc.bottom = rect.GetBottom();

	DrawText(m_MemoryDC, std::string(text.begin(), text.end()).c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SetBkMode(m_MemoryDC, OPAQUE);
}
