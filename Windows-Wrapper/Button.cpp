#include "Button.h"
#include "ControlException.h"
#include "Direct2D.h"

void Button::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
{
	auto bgColor = GetBackgroundColor();
	auto ftColor = GetForeColor();

	auto insider = rectangle;

	switch (m_FlatStyle)
	{
		case FlatStyle::Standard_Windows10:	// Standard button MouseOver and Clicking effects are pre-defineds. User can only change the background color
		{
			if (IsClicking())		// m_IsTabSelected ALWAYS true when Clicking
			{
				insider = graphics->DrawRectangle(Color(0, 84, 153), insider, 1, ChartDashStyle::Solid);
				insider = graphics->DrawRectangle(Color(204, 228, 247), insider, 1, ChartDashStyle::Dot);
				insider = graphics->DrawRectangle(Color(204, 228, 247), insider, 1, ChartDashStyle::Solid);
			}
			else if (IsMouseOver())
			{
				if (m_IsTabSelected)
				{
					insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Solid);
					insider = graphics->DrawRectangle(Color(229, 241, 251), insider, 1, ChartDashStyle::Dot);
					insider = graphics->DrawRectangle(Color(229, 241, 251), insider, 1, ChartDashStyle::Solid);
				}
				else
				{
					insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Solid);
					insider = graphics->DrawRectangle(Color(229, 241, 251), insider, 2, ChartDashStyle::Solid);
				}
			}
			else
			{
				if (m_IsTabSelected)
				{
					insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Solid);
					insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Dot);
					insider = graphics->DrawRectangle(Color(225, 225, 225), insider, 1, ChartDashStyle::Solid);
				}
				else
				{
					insider = graphics->DrawRectangle(Color(173, 173, 173), insider, 1, ChartDashStyle::Solid);
					insider = graphics->DrawRectangle(Color(222, 225, 225), insider, 2, ChartDashStyle::Solid);
				}
			}

			graphics->FillRectangle(bgColor, insider);

			break;
		}
		case FlatStyle::Standard_Windows11:
		{
			// Fill the original rectangle with parent background to remove empty black spaces
			graphics->FillRectangle(Parent->GetBackgroundColor(), rectangle);

			if (IsClicking())
			{
				insider = graphics->DrawRoundedRectangle(Color(226, 226, 226), rectangle, 1, ChartDashStyle::Solid, 5);
				
				if (GetBackgroundColor() == Color::ControlBackground_Win11()) graphics->FillRoundedRectangle(Color(243, 243, 243), insider, 1);
				else graphics->FillRectangle(Color::ControlBackground_Win11(), insider);
				
				insider.Deflate(2, 2);

				insider = graphics->DrawRectangle(bgColor, insider, 1, ChartDashStyle::Dot);

				if (GetBackgroundColor() == Color::ControlBackground_Win11()) graphics->FillRectangle(Color(243, 243, 243), insider);
				else graphics->FillRectangle(bgColor, insider);
			}
			else if (IsMouseOver())
			{
				insider = graphics->DrawRoundedRectangle(Color(0, 120, 212), rectangle, 1, ChartDashStyle::Solid, 5);
				graphics->FillRoundedRectangle(Color::ControlBackground_Win11(), insider, 1);

				insider.Deflate(2, 2);

				if (m_IsTabSelected)
				{
					insider = graphics->DrawRectangle(Color(216, 230, 241), insider, 1, ChartDashStyle::Dot);
				}

				graphics->FillRectangle(bgColor, insider);
			}
			else
			{
				if (m_IsTabSelected)
				{
					insider = graphics->DrawRoundedRectangle(Color(0, 120, 212), rectangle, 1, ChartDashStyle::Solid, 5);
					graphics->FillRoundedRectangle(Color::ControlBackground_Win11(), insider, 1);
					insider.Deflate(2, 2);
					insider = graphics->DrawRectangle(bgColor, insider, 1, ChartDashStyle::Dot);
					graphics->FillRectangle(bgColor, insider);
				}
				else
				{
					insider = graphics->DrawRoundedRectangle(Color(201, 201, 201), rectangle, 1, ChartDashStyle::Solid, 5);

					insider.Deflate(2, 2);
					graphics->FillRoundedRectangle(bgColor, insider, 1);
				}
			}

			break;
		}
	}

	graphics->CommonDrawText(Text, GetFont(), ftColor, insider);
}

int Button::OnEraseBackground_Impl(HWND hwnd, HDC hdc)
{
	return 1;	// To avoid flickering
}

void Button::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	switch (vk)
	{
		case VK_RETURN:
		{
			SetClickingState(true);
			Update();
			break;
		}
	}

	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void Button::OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags)
{
	switch (vk)
	{
		case VK_RETURN:
		{
			Dispatch("OnClick", &ArgsDefault);
			SetClickingState(false);
			Update();
			break;
		}
	}

	Control::OnKeyUp_Impl(hwnd, vk, cRepeat, flags);
}

Button::Button(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
	m_FlatStyle(FlatStyle::Standard_Windows11),
	m_FlatAppearance()
{
	Initialize();
}

Button::~Button()
{

}

void Button::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,				// Style values
		m_Location.X,										// X position
		m_Location.Y,										// Y position
		m_Size.Width,									// Width
		m_Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		nullptr,					                	// Menu handle
		WindowClass::GetInstance(),						// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}
}

FlatButtonAppearance Button::GetFlatButtonAppearance() const noexcept
{
	return m_FlatAppearance;
}

void Button::SetFlatButtonAppearance(FlatButtonAppearance appearance) noexcept
{
	m_FlatAppearance = appearance;
	Update();
}

FlatStyle Button::GetFlatStyle() const noexcept
{
	return m_FlatStyle;
}

void Button::SetFlatStyle(FlatStyle style) noexcept
{
	if (m_FlatStyle == FlatStyle::Standard_Windows11 && GetBackgroundColor() == Color::ControlBackground_Win11())
	{
		SetBackgroundColor(Color::ControlBackground_Win10());
	}
	else if (m_FlatStyle == FlatStyle::Standard_Windows10 && GetBackgroundColor() == Color::ControlBackground_Win10())
	{
		SetBackgroundColor(Color::ControlBackground_Win11());
	}

	m_FlatStyle = style;

	Update();
}
