#include "ToolStrip.h"

void ToolStrip::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{
	auto hdc = static_cast<HDC>(graphics->GetHDC().ToPointer());
	RECT rc = { 0 };
	rc.left = rectangle.GetLeft();
	rc.top = rectangle.GetTop();
	rc.right = rectangle.GetRight();
	rc.bottom = rectangle.GetBottom();

	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdc, &rc, bgColor);
	SelectObject(hdc, bgColor);
	DeleteObject(bgColor);
}

ToolStrip::ToolStrip(Control* parent)
	:
	Control(parent, 
		"",
		parent->GetSize().Width - (parent->GetMargin().Left + parent->GetMargin().Right),
		0,	// Height is calculated base on Font size
		parent->GetMargin().Left,
		parent->m_MinSize + parent->GetMargin().Top),
	m_DockStyle(DockStyle::Top),
	m_GripStyle(ToolStripGripStyle::Visible)
{
	Initialize();
}

void ToolStrip::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),																			// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,											// Style values
		m_Location.X,																							// X position
		m_Location.Y,																							// Y position
		m_Size.Width,																						// Width
		m_Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		nullptr,						                												// Menu handle
		WindowClass::GetInstance(),																		// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Set default TextBox margin to 3 pixels
	m_Margin = 3;
}

DockStyle ToolStrip::GetDockStyle() const noexcept
{
	return m_DockStyle;
}

void ToolStrip::SetDockStyle(DockStyle style) noexcept
{
	m_DockStyle = style;
	Update();
}

ToolStripGripStyle ToolStrip::GetGripStyle() const noexcept
{
	return m_GripStyle;
}

void ToolStrip::SetGripStyle(ToolStripGripStyle style) noexcept
{
	m_GripStyle = style;
	Update();
}

ToolStripRenderMode ToolStrip::GetRenderMode() const noexcept
{
	return m_Renderer;
}

void ToolStrip::SetRenderMode(ToolStripRenderMode mode) noexcept
{
	m_Renderer = mode;
	Update();
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
