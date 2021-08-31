#include "Label.h"

// Singleton ButtonClass
Label::LabelClass Label::LabelClass::m_LabelClass;

// ProgressBar class declarations
Label::LabelClass::LabelClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetName();
	RegisterClassEx(&wc);
}

Label::LabelClass::~LabelClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* Label::LabelClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE Label::LabelClass::GetInstance() noexcept
{
	return m_LabelClass.m_Instance;
}

void Label::OnPaint_Impl(HWND hwnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	SetBkMode(ps.hdc, OPAQUE);

	HFONT hFont = CreateFont(
		m_Font.GetSizeInPixels(), 
		0, 
		0, 
		0, 
		m_Font.IsBold() ? FW_BOLD : FW_NORMAL, 
		m_Font.IsItalic(), 
		m_Font.IsUnderline(), 
		m_Font.IsStrikeOut(), 
		ANSI_CHARSET,
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, 
		m_Font.GetName().c_str());

	SelectObject(ps.hdc, hFont);

	SIZE s;
	GetTextExtentPoint32(ps.hdc, Text.c_str(), static_cast<int>(Text.length()), &s);
	m_Size = Size(s.cx + m_Margin.Left + m_Margin.Right + 8, s.cy + m_Margin.Top + m_Margin.Bottom + 2);
	SetWindowPos(hwnd, HWND_TOP, Location.X, Location.Y, m_Size.Width, m_Size.Height, 0);

	RECT r;
	GetClientRect(hwnd, &r);

	// Create TextBox border
	switch (m_BorderStyle)
	{
	case BorderStyle::None:
	{
		// No border is drawn
		break;
	}
	case BorderStyle::FixedSingle:
	{
		HPEN pen;
		HGDIOBJ old_pen;

		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(99, 103, 99));
		old_pen = SelectObject(ps.hdc, pen);
		Rectangle(ps.hdc, r.left, r.top, r.right, r.bottom);

		//Clean up
		SelectObject(ps.hdc, old_pen);
		DeleteObject(old_pen);
		SelectObject(ps.hdc, pen);
		DeleteObject(pen);

		// Adjust padding for text
		r.left += 1;
		r.top += 1;
		r.right -= 1;
		r.bottom -= 1;

		break;
	}
	case BorderStyle::Fixed3D:
	{
		HPEN pen;
		HGDIOBJ old_pen;

		// Draw white border part
		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(255, 255, 255));
		old_pen = SelectObject(ps.hdc, pen);
		Rectangle(ps.hdc, r.left, r.top, r.right, r.bottom);

		// Draw gray border part
		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(160, 160, 160));
		old_pen = SelectObject(ps.hdc, pen);
		Rectangle(ps.hdc, r.left, r.top, r.right - 1 , r.bottom - 1);

		// Adjust padding for text
		r.left += 1;
		r.top += 1;
		r.right -= 1;
		r.bottom -= 1;

		//Clean up
		SelectObject(ps.hdc, old_pen);
		DeleteObject(old_pen);
		SelectObject(ps.hdc, pen);
		DeleteObject(pen);

		break;
	}
	}

	switch (m_FlatStyle)
	{
	case FlatStyle::Flat: break;
	case FlatStyle::Popup: break;
	case FlatStyle::Standard: break;
	case FlatStyle::System: m_Margin = 1; break; // FlatStyle is forced to left
	}

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(ps.hdc, &r, bgColor);
	SelectObject(ps.hdc, bgColor);
	DeleteObject(bgColor);

	r.left += m_Margin.Left;
	r.top += m_Margin.Top;
	r.right -= m_Margin.Right;
	r.bottom -= m_Margin.Bottom;

	if (m_Font.IsItalic() && m_FlatStyle != FlatStyle::System)
	{
		r.left -= 2;
		r.right += 2;
	}

	// Adjust text in center based on default label size with font size as 1
	//int spacingForAlignment = r.bottom - m_Font.GetSizeInPixels();
	//r.top += (spacingForAlignment / 2) - 4;	// 4 Pixels extra for small TextBox and underline characteres

	// Draw label text
	SetBkMode(ps.hdc, TRANSPARENT);
	SetTextColor(ps.hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
	DrawText(ps.hdc, Text.c_str(), -1, &r, DT_LEFT | DT_VCENTER);
	DrawText(ps.hdc, Text.c_str(), static_cast<int>(Text.length()), &r, DT_LEFT | DT_VCENTER | DT_CALCRECT);
	DeleteObject(hFont);

	EndPaint(hwnd, &ps);
}

Label::Label(Control* parent, const std::string& text, int x, int y)
	:
	Control(parent, text, 0, 0, x, y),
	m_BorderStyle(BorderStyle::None),
	m_FlatStyle(FlatStyle::Standard)
{
	Initialize();
}

void Label::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		LabelClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,				// Style values
		Location.X,										// X position
		Location.Y,										// Y position
		m_Size.Width,										// Width
		m_Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		NULL,						                	// Menu handle
		LabelClass::GetInstance(),						// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
}

BorderStyle Label::GetBorderStyle() const noexcept
{
	return m_BorderStyle;
}

void Label::SetBorderStyle(BorderStyle style) noexcept
{
	m_BorderStyle = style;
	Update();
}

FlatStyle Label::GetFlatStyle() const noexcept
{
	return m_FlatStyle;
}

void Label::SetFlatStyle(FlatStyle style) noexcept
{
	m_FlatStyle = style;
	Update();
}
