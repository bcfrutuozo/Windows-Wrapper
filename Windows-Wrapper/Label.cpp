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

	HFONT hFont = CreateFont(Font.GetSizeInPixels(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());

	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(ps.hdc, &ps.rcPaint, bgColor);
	SelectObject(ps.hdc, bgColor);
	DeleteObject(bgColor);

	EndPaint(hwnd, &ps);
}

Label::Label(Control* parent, const std::string& text, int x, int y)
	:
	Control(parent, text, 0, 0, x, y)
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