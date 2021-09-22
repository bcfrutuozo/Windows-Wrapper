#include "ProgressBar.h"

int ProgressBar::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;	// Reduce control flickering
}

void ProgressBar::OnPaint_Impl(HWND hwnd) noexcept
{
	// Process default paint only when thread is not running
	if (m_IsRunning)
	{
		return;
	}

	PAINTSTRUCT ps;
	RECT rt;
	GetClientRect(hwnd, &rt);
	BeginPaint(hwnd, &ps);
	HDC hdcMem = CreateCompatibleDC(ps.hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, m_Size.Width, m_Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	switch (m_Animation)
	{
	case ProgressBarAnimation::Blocks:
	{
		HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(188, 188, 188));
		HGDIOBJ draw = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, rt.left, rt.top, rt.right, rt.bottom);
		DeleteObject(draw);
		SelectObject(hdcMem, pen);
		DeleteObject(pen);

		// Enter rectangle ignoring border
		rt.left += 1;
		rt.top += 1;
		rt.right -= 1;
		rt.bottom -= 1;

		char buff[100];
		wsprintf(buff, "%d%% Finished", m_Value);
		SIZE size;
		GetTextExtentPoint32(hdcMem, buff, ::lstrlen(buff), &size);

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

			SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			SetBkColor(hdcMem, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
			ExtTextOut(hdcMem, (right - size.cx) / 2, (rt.bottom - size.cy) / 2, ETO_CLIPPED | ETO_OPAQUE, &rt, buff, lstrlen(buff), NULL);
		}

		// Draw the remaining amount region
		if (m_Value < Maximum)
		{
			SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			SetBkColor(hdcMem, RGB(230, 230, 230));
			rt.left = rt.right;
			rt.right = right;
			ExtTextOut(hdcMem, (right - size.cx) / 2, (rt.bottom - size.cy) / 2, ETO_CLIPPED | ETO_OPAQUE, &rt, buff, lstrlen(buff), NULL);
		}

		break;
	}
	case ProgressBarAnimation::Marquee:	// Marquee ProgressBar is painted on another thread
	{

		HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(188, 188, 188));
		HGDIOBJ draw = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, rt.left, rt.top, rt.right, rt.bottom);
		rt.left += 1;
		rt.top += 1;
		rt.right -= 1;
		rt.bottom -= 1;

		HBRUSH bgColor = CreateSolidBrush(RGB(230, 230, 230));
		FillRect(hdcMem, &rt, bgColor);
		DeleteObject(draw);
		SelectObject(hdcMem, bgColor);
		DeleteObject(bgColor);
		SelectObject(hdcMem, pen);
		DeleteObject(pen);
		break;
	}
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(ps.hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
	EndPaint(hwnd, &ps);
}

void ProgressBar::OnPaintMarquee_Thread(HWND hwnd) noexcept
{
	HDC hdc = GetDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBRUSH defaultControlColor = CreateSolidBrush(RGB(230, 230, 230));
	HBRUSH bgColor = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));

	while (m_IsRunning)
	{
		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, m_Size.Width, m_Size.Height);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

		long long time = (m_Speed * 2.0f) * m_Speed;
		std::this_thread::sleep_for(std::chrono::microseconds(time));

		RECT rt;
		GetClientRect(hwnd, &rt);

		HPEN pen = CreatePen(PS_INSIDEFRAME, 1, RGB(188, 188, 188));
		HGDIOBJ draw = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, rt.left, rt.top, rt.right, rt.bottom);
		DeleteObject(pen);
		SelectObject(hdcMem, draw);
		DeleteObject(draw);

		// Enter rectangle ignoring border
		rt.left += 1;
		rt.top += 1;
		rt.right -= 1;
		rt.bottom -= 1;

		// Reset Marquee effect when inner bar leaves the main bar window
		if (Minimum >= m_Size.Width)
		{
			Minimum = -125;
			Maximum = 0;
			FillRect(hdcMem, &rt, defaultControlColor);
		}
		else
		{
			// Draw first portion if Marquee effect is in middle of the bar
			if (Maximum - Minimum < 1)
			{
				rt.right = Minimum;
				FillRect(hdcMem, &rt, defaultControlColor);
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
					rt.left = Minimum - 1;
				}

				if (Maximum > m_Size.Width - 1)
				{
					rt.right = m_Size.Width - 1;
				}
				else
				{
					rt.right = Maximum + 1;
				}

				FillRect(hdcMem, &rt, bgColor);
			}

			// Draw the end of the bar if Marquee effect is in middle
			if (Maximum < m_Size.Width - 1)
			{
				rt.left = Maximum + 1;
				rt.right = m_Size.Width - 1;

				FillRect(hdcMem, &rt, defaultControlColor);
			}

			// Redraw back part of the bar
			if (Minimum > 1)
			{
				rt.left = 1;
				rt.right = Minimum - 1;
				FillRect(hdcMem, &rt, defaultControlColor);
			}

			// Increment Marquee effect 1 pixel for each draw
			Minimum += 1;
			Maximum += 1;
		}

		// Perform the bit-block transfer between the memory Device Context which has the next bitmap
		// with the current image to avoid flickering
		BitBlt(hdc, 0, 0, m_Size.Width, m_Size.Height, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdc, hbmMem);
		DeleteObject(hbmMem);
		SelectObject(hdcMem, hbmOld);
		DeleteObject(hbmOld);
	}

	SelectObject(hdc, defaultControlColor);
	DeleteObject(defaultControlColor);
	SelectObject(hdc, bgColor);
	DeleteObject(bgColor);
	ReleaseDC(hwnd, hdcMem);
	DeleteDC(hdcMem);
	ReleaseDC(hwnd, hdc);
	DeleteDC(hdc);
}

ProgressBar::ProgressBar(Control* parent, int width, int height, int x, int y)
	:
	ProgressBar(parent, "", width, height, x, y)
{

}

ProgressBar::ProgressBar(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	Control(parent, name, width, height, x, y),
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
	// Waits to update thread to finish in order to close the application without code 3
	if (m_IsRunning)
	{
		m_IsRunning = false;
		m_UpdateThread.join();
	}
}

void ProgressBar::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		WindowClass::GetName(),							// Class name
		Text.c_str(),									// Window title
		WS_CHILD | WS_VISIBLE,							// Style values
		m_Location.X,										// X position
		m_Location.Y,										// Y position
		m_Size.Width,									// Width
		m_Size.Height,									// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),	// Parent handle
		nullptr,						                // Menu handle
		WindowClass::GetInstance(),						// Module instance handle
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
		Update();
	}
	else
	{
		if (value > Maximum && m_Value < Maximum)
		{
			m_Value = Maximum;
			Update();
		}

		if (value < Minimum && m_Value > Minimum)
		{
			m_Value = Minimum;
			Update();
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

void ProgressBar::SetSpeed(int value) noexcept
{
	if (m_Animation != ProgressBarAnimation::Marquee)
	{
		return;
	}

	if (m_Speed > 100)
	{
		m_Speed = 100;
	}
	else if (m_Speed < 1)
	{
		m_Speed = 1;
	}
	else
	{
		m_Speed = value;
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
		m_Speed = 0;
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
		m_Speed = 50;
		break;
	}
	}

	m_Animation = animation;
	Update();
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
			Update();
		}
	}
}