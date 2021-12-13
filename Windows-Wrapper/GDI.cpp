//#include "GDI.h"
//#include "Enums.h"
//#include "Exceptions.h"
//
//GDI::GDI(IntPtr window, Size size)
//	:
//	Graphics(window, size),
//	m_PS({ 0 }),
//	m_MemoryBitmap(nullptr),
//	m_MemoryDC(nullptr),
//	m_OldBitmap(nullptr)
//{
//	DCHandle = GetDC(static_cast<HWND>(window.ToPointer()));
//}
//
//void GDI::ClearResources()
//{
//	for (auto p : Elements)
//	{
//		auto obj = (HGDIOBJ)&p;
//		SelectObject(static_cast<HDC>(GetHDC().ToPointer()), obj);
//		DeleteObject(obj);
//	}
//
//	ReleaseDC(static_cast<HWND>(WindowHandle.ToPointer()), static_cast<HDC>(DCHandle.ToPointer()));
//	DeleteDC(static_cast<HDC>(DCHandle.ToPointer()));
//}
//
//void GDI::BeginDraw()
//{
//	DCHandle = BeginPaint(static_cast<HWND>(WindowHandle.ToPointer()), &m_PS);
//	m_MemoryDC = CreateCompatibleDC(static_cast<HDC>(DCHandle.ToPointer()));
//	m_MemoryBitmap = CreateCompatibleBitmap(static_cast<HDC>(DCHandle.ToPointer()), WindowSize.Width, WindowSize.Height);
//	m_OldBitmap = (HBITMAP)SelectObject(m_MemoryDC, m_MemoryBitmap);
//}
//
//void GDI::EndDraw()
//{
//	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
//	// with the current image to avoid flickering
//	BitBlt(static_cast<HDC>(DCHandle.ToPointer()), 0, 0, WindowSize.Width, WindowSize.Height, m_MemoryDC, 0, 0, SRCCOPY);
//	SelectObject(m_MemoryDC, m_OldBitmap);
//	ReleaseDC(static_cast<HWND>(WindowHandle.ToPointer()), m_MemoryDC);
//	DeleteDC(m_MemoryDC);
//
//	EndPaint(static_cast<HWND>(WindowHandle.ToPointer()), &m_PS);
//}
//
//const IntPtr GDI::CreateSolidBrush(Color c)
//{
//	HBRUSH brush;
//
//	if (Elements.contains(c.ToString()))
//	{
//		brush = (HBRUSH)GetElement(std::string(c.ToString()));
//
//		if (!brush) throw ArgumentException("Element already exist but it's not from HBRUSH type");
//
//		return brush;
//	}
//
//	brush = ::CreateSolidBrush(c.ToRGB());
//
//	if (!brush) throw ExternalException("Could not create solid color brush", GetLastError());
//
//	Elements.insert(std::pair<std::string, void*>(c.ToString(), brush));
//
//	return brush;
//}
//
//const IntPtr GDI::CreateFontObject(const Font& f)
//{
//	HFONT font;
//
//	if (Elements.contains(f.ToString()))
//	{
//		font = (HFONT)GetElement(std::string(f.ToString()));
//
//		if (!font) throw ArgumentException("Element already exist but it's not from HFONT type");
//
//		return font;
//	}
//
//	font = ::CreateFont(
//		f.GetSizeInPixels(),
//		0,
//		0,
//		0,
//		f.IsBold() ? FW_BOLD : FW_NORMAL,
//		f.IsItalic(),
//		f.IsUnderline(),
//		f.IsStrikeOut(),
//		ANSI_CHARSET,
//		OUT_TT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//		DEFAULT_PITCH | FF_DONTCARE,
//		f.GetName().c_str());
//
//	if (!font) throw ExternalException("Could not create font", GetLastError());
//
//	Elements.insert(std::pair<std::string, void*>(f.ToString(), font));
//
//	return font;
//}
//
//Drawing::Rectangle GDI::DrawRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle)
//{
//	auto brush = Elements.contains(c.ToString()) ? Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();
//
//	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");
//
//	RECT rc = { rect.X, rect.Y, rect.Width, rect.Height };
//
//	int bordergdi = -1;
//
//	switch (borderStyle)
//	{
//		case ChartDashStyle::Dash: bordergdi = PS_DASH; break;
//		case ChartDashStyle::DashDot: bordergdi = PS_DASHDOT;  break;
//		case ChartDashStyle::DashDotDot: bordergdi = PS_DASHDOTDOT; break;
//		case ChartDashStyle::Dot: bordergdi = PS_DOT; break;
//		case ChartDashStyle::Solid: bordergdi = PS_SOLID; break;
//		default:
//		throw ArgumentNullException("borderStyle");
//	}
//
//	HPEN pen = nullptr;
//	HGDIOBJ old_pen = nullptr;
//
//	SetBkMode(m_MemoryDC, OPAQUE);
//
//	if (borderStyle != ChartDashStyle::Solid)
//	{
//		pen = CreatePen(bordergdi, 1, Color::Black().ToRGB());
//		old_pen = SelectObject(m_MemoryDC, pen);
//	}
//	else
//	{
//		pen = CreatePen(PS_SOLID, borderSize, c.ToRGB());
//		old_pen = SelectObject(m_MemoryDC, pen);
//	}
//
//	SetBkColor(m_MemoryDC, c.ToRGB());
//	SetBkMode(m_MemoryDC, OPAQUE);
//	Rectangle(m_MemoryDC, rc.left, rc.top, rc.right, rc.bottom);
//
//	//Clean up
//	SelectObject(m_MemoryDC, old_pen);
//	DeleteObject(old_pen);
//	SelectObject(m_MemoryDC, pen);
//	DeleteObject(pen);
//
//	return Drawing::Rectangle::Deflate(rect, borderSize, borderSize);
//}
//
//Drawing::Rectangle GDI::DrawRoundedRectangle(Color c, Drawing::Rectangle rect, int borderSize, ChartDashStyle borderStyle, int radius)
//{
//	auto brush = Elements.contains(c.ToString()) ? Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();
//
//	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");
//
//	RECT rc = { rect.X, rect.Y, rect.Width, rect.Height };
//
//	int bordergdi = -1;
//
//	switch (borderStyle)
//	{
//		case ChartDashStyle::Dash: bordergdi = PS_DASH; break;
//		case ChartDashStyle::DashDot: bordergdi = PS_DASHDOT;  break;
//		case ChartDashStyle::DashDotDot: bordergdi = PS_DASHDOTDOT; break;
//		case ChartDashStyle::Dot: bordergdi = PS_DOT; break;
//		case ChartDashStyle::Solid: bordergdi = PS_SOLID; break;
//		default:
//		throw ArgumentNullException("borderStyle");
//	}
//
//	HPEN pen = nullptr;
//	HGDIOBJ old_pen = nullptr;
//
//	SetBkMode(m_MemoryDC, TRANSPARENT);
//
//	if (borderStyle != ChartDashStyle::Solid)
//	{
//		pen = CreatePen(bordergdi, 0, Color::Black().ToRGB());
//		old_pen = SelectObject(m_MemoryDC, pen);
//	}
//	else
//	{
//		pen = CreatePen(PS_SOLID, borderSize, c.ToRGB());
//		old_pen = SelectObject(m_MemoryDC, pen);
//	}
//
//	SetBkColor(m_MemoryDC, c.ToRGB());
//	SetBkMode(m_MemoryDC, TRANSPARENT);
//	RoundRect(m_MemoryDC, rc.left, rc.top, rc.right, rc.bottom, radius, radius);
//
//	//Clean up
//	SelectObject(m_MemoryDC, old_pen);
//	DeleteObject(old_pen);
//	SelectObject(m_MemoryDC, pen);
//	DeleteObject(pen);
//
//	return Drawing::Rectangle::Deflate(rect, borderSize, borderSize);
//}
//
//void GDI::FillRectangle(Color c, Drawing::Rectangle rect)
//{
//	auto brush = Elements.contains(c.ToString()) ? (HBRUSH)Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();
//
//	if (!brush) throw InvalidCastException("Cannot convert the following item to brush type!");
//
//	RECT rc = { rect.X, rect.Y, rect.Width, rect.Height };
//
//	FillRect(m_MemoryDC, &rc, brush);
//}
//
//void GDI::FillRoundedRectangle(Color c, Drawing::Rectangle rect, int radius)
//{
//	auto brush = Elements.contains(c.ToString()) ? (HBRUSH)Elements.at(c.ToString()) : (HBRUSH)CreateSolidBrush(c).ToPointer();
//
//	RECT rcLeftToRight = { rect.X, rect.Y + radius, rect.Width, rect.Height - radius };
//
//	FillRect(m_MemoryDC, &rcLeftToRight, brush);
//
//	RECT rcTopToBottom{ rect.X + radius, rect.Y, rect.Width - radius, rect.Height };
//
//	FillRect(m_MemoryDC, &rcTopToBottom, brush);
//}
//
//Size GDI::GetTextSize(const std::string& text, const Font& font)
//{
//	return GetTextSize(text.c_str(), static_cast<int>(text.length()), font);
//}
//
//Size GDI::GetTextSize(const char* text, int length, const Font& font)
//{
//	SIZE s;
//	auto winFont = Elements.contains(font.ToString()) ? Elements.at(font.ToString()) : (HFONT)CreateFontObject(font).ToPointer();
//	SelectObject(static_cast<HDC>(DCHandle.ToPointer()), winFont);
//	GetTextExtentPoint32(static_cast<HDC>(DCHandle.ToPointer()), text, length, &s);
//	return Size(s.cx, s.cy);
//}
//
//Drawing::Rectangle GDI::DrawTransparentText(const std::string& text, HorizontalAlignment alignment, const Font& font, Color foreColor, Drawing::Rectangle rect, size_t cursorIndex)
//{
//	auto wasOpaque = GetBkMode(m_MemoryDC) == OPAQUE;
//
//	auto winFont = Elements.contains(font.ToString()) ? Elements.at(font.ToString()) : (HFONT)CreateFontObject(font).ToPointer();
//	SelectObject(m_MemoryDC, winFont);
//
//	if (wasOpaque) SetBkMode(m_MemoryDC, TRANSPARENT);
//
//	SetTextColor(m_MemoryDC, foreColor.ToRGB());
//
//	RECT rc = { rect.X, rect.Y, rect.Width, rect.Height };
//
//	unsigned int align = 0;
//
//	switch (alignment)
//	{
//		case HorizontalAlignment::Left: align = DT_LEFT; break;
//		case HorizontalAlignment::Right: align = DT_RIGHT; break;
//		case HorizontalAlignment::Center: align = DT_CENTER; break;
//	}
//
//	DrawText(m_MemoryDC, text.c_str(), -1, &rc, align | DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL);
//	DrawText(m_MemoryDC, text.c_str(), static_cast<int>(cursorIndex == 0 ? text.length() : cursorIndex), &rc, align | DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_CALCRECT);
//
//	if (wasOpaque) SetBkMode(m_MemoryDC, OPAQUE);
//
//	return Drawing::Rectangle(rc.left, rc.top, rc.right, rc.bottom);
//}
//
//Drawing::Rectangle GDI::DrawOpaqueText(const std::string& text, HorizontalAlignment alignment, const Font& font, Color foreColor, Drawing::Rectangle rect, Color backColor, size_t cursorIndex)
//{
//	auto wasTransparent = GetBkMode(m_MemoryDC) == TRANSPARENT;
//
//	auto winFont = Elements.contains(font.ToString()) ? Elements.at(font.ToString()) : (HFONT)CreateFontObject(font).ToPointer();
//	SelectObject(m_MemoryDC, winFont);
//
//	if (wasTransparent) SetBkMode(m_MemoryDC, OPAQUE);
//
//	SetTextColor(m_MemoryDC, foreColor.ToRGB());
//	SetBkColor(m_MemoryDC, backColor.ToRGB());
//
//	RECT rc = { rect.X, rect.Y, rect.Width, rect.Height };
//
//	unsigned int align = 0;
//
//	switch (alignment)
//	{
//		case HorizontalAlignment::Left: align = DT_LEFT; break;
//		case HorizontalAlignment::Right: align = DT_RIGHT; break;
//		case HorizontalAlignment::Center: align = DT_CENTER; break;
//	}
//
//	DrawText(m_MemoryDC, text.c_str(), -1, &rc, align | DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL);
//	DrawText(m_MemoryDC, text.c_str(), static_cast<int>(cursorIndex == 0 ? text.length() : cursorIndex), &rc, align | DT_VCENTER | DT_SINGLELINE | DT_EDITCONTROL | DT_CALCRECT);
//
//	if (wasTransparent) SetBkMode(m_MemoryDC, TRANSPARENT);
//
//	return Drawing::Rectangle(rc.left, rc.top, rc.right, rc.bottom);
//}
