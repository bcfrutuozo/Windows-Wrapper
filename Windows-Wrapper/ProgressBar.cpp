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

int ProgressBar::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;	// Reduce control flickering
}

void ProgressBar::OnPaint_Impl(HWND hwnd) noexcept
{
	switch (m_Animation)
	{
	case ProgressBarAnimation::Blocks:
	{
		RECT rt;
		GetClientRect(hwnd, &rt);
		HDC hdc = GetDC(hwnd);
		if (hdc == nullptr)
		{
			break;
		}

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
		}

		ReleaseDC(hwnd, hdc);

		break;
	}
	case ProgressBarAnimation::Marquee:	// Marquee ProgressBar is painted on another thread
	{
		// Draw Marquee effect ProgressBar when it's not running
		if (!m_IsRunning)
		{
			RECT rt;
			GetClientRect(hwnd, &rt);
			HDC hdc = GetDC(hwnd);
			if (hdc == nullptr)
			{
				break;
			}

			HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(188, 188, 188));
			HGDIOBJ draw = SelectObject(hdc, pen);
			Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
			rt.left += 1;
			rt.top += 1;
			rt.right -= 1;
			rt.bottom -= 1;

			FillRect(hdc, &rt, CreateSolidBrush(RGB(240, 240, 240)));
			DeleteObject(draw);
			DeleteObject(pen);
			ReleaseDC(hwnd, hdc);
		}
		break;
	}
	}
}

void ProgressBar::OnPaintMarquee_Thread(HWND hwnd) noexcept
{
	HDC hdc = GetDC(hwnd);
	while (m_IsRunning)
	{
		RECT rt;
		GetClientRect(hwnd, &rt);

		// Get the HDC object without forcing it everytime
		if (hdc == nullptr)
		{
			break;
		}

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

		// Draw first portion if Marquee effect is in middle of the bar
		if (Maximum - Minimum < 1)
		{
			rt.right = Minimum;
			FillRect(hdc, &rt, CreateSolidBrush(RGB(240, 240, 240)));
		}

		// Draw inner bar for Marquee effect
		if (Maximum > 1)
		{
			if (Minimum < 1)
			{
				rt.left = 1;
			}
			else
			{
				rt.left = Minimum;
			}

			if (Maximum > Size.Width - 1)
			{
				rt.right = Size.Width - 1;
			}
			else
			{
				rt.right = Maximum - 1;
			}

			FillRect(hdc, &rt, CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB())));
		}

		// Draw the end of the bar if Marquee effect is in middle
		if (Maximum < Size.Width - 1)
		{
			rt.left = rt.right;
			rt.right = Size.Width - 1;

			FillRect(hdc, &rt, CreateSolidBrush(RGB(240, 240, 240)));
		}

		// Redraw back part of the bar
		if (Minimum > 1)
		{
			RECT o;
			CopyRect(&o, &rt);
			o.left = 2;
			o.right = Minimum - 1;
			FillRect(hdc, &o, CreateSolidBrush(RGB(240, 240, 240)));
		}

		// Increment Marquee effect 1 pixel for each draw
		Minimum += 1;
		Maximum += 1;

		// Reset Marquee effect when inner bar leaves the main bar window
		if (Minimum >= Size.Width)
		{
			Minimum = -125;
			Maximum = 0;
		}

		Sleep(1);
	}

	if (hdc != nullptr)
	{
		ReleaseDC(hwnd, hdc);
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
	Maximum(100),
	m_IsRunning(false),
	m_Animation(ProgressBarAnimation::Blocks)
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

	m_BackgroundColor = Color(75, 154, 255);
	m_ForeColor = Color::WindowText();
}

int ProgressBar::GetValue() const noexcept
{
	if (m_Animation == ProgressBarAnimation::Marquee)
	{
		return 0;
	}

	return m_Value;
}

void ProgressBar::SetValue(int value) noexcept
{
	if (m_Animation == ProgressBarAnimation::Marquee)
	{
		return;
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
		}

		if (value < Minimum && m_Value > Minimum)
		{
			m_Value = Minimum;
			InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
		}
	}
}

void ProgressBar::IncrementStep() noexcept
{
	if (m_Animation != ProgressBarAnimation::Marquee)
	{
		SetValue(m_Value + Step);
	}
}

void ProgressBar::DecrementStep() noexcept
{
	if (m_Animation != ProgressBarAnimation::Marquee)
	{
		SetValue(m_Value - Step);
	}
}

void ProgressBar::SetMinimum(int value) noexcept
{
	if (m_Animation != ProgressBarAnimation::Marquee)
	{
		Minimum = value;
	}
}

void ProgressBar::SetMaximum(int value) noexcept
{
	if (m_Animation != ProgressBarAnimation::Marquee)
	{
		Maximum = value;
	}
}

ProgressBarAnimation ProgressBar::GetAnimationType() const noexcept
{
	return m_Animation;
}

void ProgressBar::SetAnimation(ProgressBarAnimation animation) noexcept
{
	if (animation == m_Animation)
	{
		return;
	}

	switch (animation)
	{
	case ProgressBarAnimation::Blocks:
	{
		// Stop Marquee effect if it's running
		if (m_IsRunning)
		{
			m_IsRunning = false;
			m_UpdateThread.join();
		}
		Minimum = 0;
		Maximum = 100;
		Step = 10;
		m_Value = 0;
		break;
	}
	case ProgressBarAnimation::Marquee:
	{
		// When animation is set to Marquee, the ProgressBar fields are used
		// to manipulate the effect. The user can't change them.
		Minimum = -125;
		Maximum = 0;
		Step = 1;
		m_Value = -125;
		break;
	}
	}

	m_Animation = animation;
	InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
}

void ProgressBar::Start() noexcept
{
	if (m_Animation == ProgressBarAnimation::Marquee)
	{
		if (!m_IsRunning)
		{
			m_IsRunning = true;
			m_UpdateThread = std::thread([this] { OnPaintMarquee_Thread(static_cast<HWND>(Handle.ToPointer())); });
		}
	}
}

void ProgressBar::Stop() noexcept
{
	if (m_Animation == ProgressBarAnimation::Marquee)
	{
		if (m_IsRunning)
		{
			m_IsRunning = false;
			m_UpdateThread.join();
			Minimum = -125;
			Maximum = 0;
			Step = 1;
			m_Value = -125;
			InvalidateRect(static_cast<HWND>(Handle.ToPointer()), nullptr, true);
		}
	}
}