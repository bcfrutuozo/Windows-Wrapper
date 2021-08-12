#include "ProgressBar.h"

// Singleton ProgressBarClass
ProgressBar::ProgressBarClass ProgressBar::ProgressBarClass::m_ProgressBarClass;

// ProgressBar class declarations
ProgressBar::ProgressBarClass::ProgressBarClass() noexcept
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

ProgressBar::ProgressBarClass::~ProgressBarClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* ProgressBar::ProgressBarClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE ProgressBar::ProgressBarClass::GetInstance() noexcept
{
	return m_ProgressBarClass.m_Instance;
}

void ProgressBar::OnPaint_Impl(HWND hwnd) noexcept
{
	RECT rt;
	GetClientRect(hwnd, &rt);
	HDC hdc = GetDC(hwnd);
	if (hdc)
	{
		HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(188, 188, 188));
		HGDIOBJ draw = SelectObject(hdc, pen);
		Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
		DeleteObject(draw);
		DeleteObject(pen);

		// Enter rectangle ignoring border
		rt.left += 1;
		rt.top += 1;
		rt.right -= 1;
		rt.bottom -= 1;

		char buff[100];
		wsprintf(buff, "%d%% Finished", m_Value);
		SIZE size;
		GetTextExtentPoint32(hdc, buff, ::lstrlen(buff), &size);
		
		// Draw the value amount region
		LONG right = rt.right;
		if (m_Value >= Minimum)
		{
			rt.right = MulDiv(m_Value, rt.right, Maximum);

			// Plus 1 on rectangle right so it doesn't destroy the left side border
			if (m_Value == 0)
			{
				++rt.right;
			}

			SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
			ExtTextOut(hdc, (right - size.cx) / 2, (rt.bottom - size.cy) / 2, ETO_CLIPPED | ETO_OPAQUE, &rt, buff, lstrlen(buff), NULL);
		}

		// Draw the remaining amount region
		if (m_Value < Maximum)
		{
			SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			SetBkColor(hdc, RGB(230, 230, 230));
			rt.left = rt.right;
			rt.right = right;
			ExtTextOut(hdc, (right - size.cx) / 2, (rt.bottom - size.cy) / 2, ETO_CLIPPED | ETO_OPAQUE, &rt, buff, lstrlen(buff), NULL);
			ReleaseDC(hwnd, hdc);
		}
	}
}

ProgressBar::ProgressBar(Control* parent, int width, int height, int x, int y)
	:
	ProgressBar(parent, "", width, height, x, y)
{

}

ProgressBar::ProgressBar(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	WinControl(parent, name, width, height, x, y),
	m_Value(0),
	Step(10),
	Minimum(0),
	Maximum(100)
{
	Initialize();
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::Initialize() noexcept
{
	// Create window and get its handle
	Handle = CreateWindow(
		ProgressBarClass::GetName(),					// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE,							// Style values
		Location.X,										// X position
		Location.Y,										// Y position
		Size.Width,										// Width
		Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		NULL,						                	// Menu handle
		ProgressBarClass::GetInstance(),				// Module instance handle
		this											// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	m_BackgroundColor = Color::Green();
	m_ForeColor = Color::White();
}

unsigned int ProgressBar::GetValue() const noexcept
{
	return m_Value;
}

void ProgressBar::SetValue(int value) noexcept
{
	// Transform signed value into unsigned to avoid value overflow
	if (value < 0)
	{
		value = 0;
	}

	if (value >= Minimum && value <= Maximum)
	{
		m_Value = value;

		// Force progress bar redraw
		InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
	}
	else
	{
		if (value > Maximum && m_Value < Maximum)
		{
			m_Value = Maximum;
			InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
			return;
		}
		
		if(value < Minimum && m_Value > Minimum)
		{
			m_Value = Minimum;
			InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
		}
	}
}

void ProgressBar::IncrementStep() noexcept
{
	SetValue(m_Value + Step);
}

void ProgressBar::DecrementStep() noexcept
{
	SetValue(m_Value - Step);
}
