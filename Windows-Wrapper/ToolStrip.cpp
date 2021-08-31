#include "ToolStrip.h"

// Singleton ButtonClass
ToolStrip::ToolStripClass ToolStrip::ToolStripClass::m_ToolStripClass;

// ToolStrip class declarations
ToolStrip::ToolStripClass::ToolStripClass() noexcept
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

ToolStrip::ToolStripClass::~ToolStripClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* ToolStrip::ToolStripClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE ToolStrip::ToolStripClass::GetInstance() noexcept
{
	return m_ToolStripClass.m_Instance;
}

void ToolStrip::OnPaint_Impl(HWND hWnd) noexcept
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	HFONT hFont = CreateFont(m_Font.GetSizeInPixels(),
		0, 
		0, 
		0, 
		m_Font.IsBold() ? FW_BOLD : FW_NORMAL, 
		m_Font.IsItalic(), 
		m_Font.IsUnderline(), 
		m_Font.IsStrikeOut(),
		ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_Font.GetName().c_str());

	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(ps.hdc, &ps.rcPaint, bgColor);
	SelectObject(ps.hdc, bgColor);
	DeleteObject(bgColor);

	EndPaint(hWnd, &ps);
}

ToolStrip::ToolStrip(Control* parent)
	:
	Control(parent, 
		"",
		parent->GetSize().Width - (parent->GetMargin().Left + parent->GetMargin().Right),
		0,	// Height is calculated base on Font size
		parent->GetMargin().Left,
		parent->m_MinSize + parent->GetMargin().Top)
{
	Initialize();
}

void ToolStrip::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		ToolStripClass::GetName(),																			// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,															// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		m_Size.Width,																						// Width
		m_Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                												// Menu handle
		ToolStripClass::GetInstance(),																		// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Set default TextBox margin to 3 pixels
	m_Margin = 3;
	m_BackgroundColor = Color::Control();
	m_ForeColor = Color::WindowText();
}

//Button& ToolStrip::AddButton(const std::string& text) noexcept
//{
//	return Create<Button>(this, text)
//	// TODO: insert return statement here
//}
//
//ProgressBar& ToolStrip::AddProgressBar() noexcept
//{
//	// TODO: insert return statement here
//}
//
//TextBox& ToolStrip::AddTextBox() noexcept
//{
//	// TODO: insert return statement here
//}
//
//Label& ToolStrip::AddLabel() noexcept
//{
//	// TODO: insert return statement here
//}
