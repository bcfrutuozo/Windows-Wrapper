#include "TextBox.h"

#include <algorithm>

// Singleton TextBoxClass
TextBox::TextBoxClass TextBox::TextBoxClass::m_TextBoxClass;

// TextBox class declarations
TextBox::TextBoxClass::TextBoxClass() noexcept
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

TextBox::TextBoxClass::~TextBoxClass()
{
	UnregisterClass(m_ClassName, GetInstance());
}

const char* TextBox::TextBoxClass::GetName() noexcept
{
	return m_ClassName;
}

HINSTANCE TextBox::TextBoxClass::GetInstance() noexcept
{
	return m_TextBoxClass.m_Instance;
}

int TextBox::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
{
	return 1;   // Returns 1 to avoid flickering
}

int TextBox::OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept
{
	return DLGC_WANTALLKEYS | DLGC_WANTARROWS;
}

void TextBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
{
	/**************************************************************************************************/
	/* This is the most important function of the TextBox class. It's responsible to manage the user
	input between the cursor and selection. By doing it so, it manages all navigation inside the TextBox
	while handles and stores the indices for common delete operations, using Backspace and Delete keys.

	To understand how selection and cursor index works:

		OnEnter

		Cursor:		        ↓
		Select:		        ↓
		Index:		0 1 2 3 4 5 6 7
		Text:		T e x t B o x \0

		OnLeftKeyPress (WITHOUT SHIFT)

		Cursor:		      ↓					Can't go lesser than 0
		Select:		      ↓					Can't go lesser than 0
		Index:		0 1 2 3 4 5 6 7
		Text:		T e x t B o x \0

		OnLeftKeyPress (WITH SHIFT)

		Cursor:		    ↓					Can't go lesser than 0
		Select:		        ↓
		Index:		0 1 2 3 4 5 6 7
		Text:		T e x t B o x \0

		OnRightKeyPress (WITHOUT SHIFT)

		Cursor:		          ↓				Can't go higher than the string.length() = '\0'
		Select:		          ↓				Can't go higher than the string.length() = '\0'
		Index:		0 1 2 3 4 5 6 7
		Text:		T e x t B o x \0

		OnLeftKeyPress (WITH SHIFT)

		Cursor:		            ↓			Can't go higher than the string.length() = '\0'
		Select:		        ↓
		Index:		0 1 2 3 4 5 6 7
		Text:		T e x t B o x \0

	/**************************************************************************************************/

	switch (vk)
	{
	case 'A':		// Gives the TextBox the CTRL+A feature to select all text from the control
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			m_SelectIndex = 0;
			m_CursorIndex = Text.length();
		}

		InputRedraw(hwnd);
		break;
	}
	case 'V':		// Gives the TextBox the CTRL+V feature to paste the data from the clipboard to the control
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			PasteFromClipboard();
		}

		InputRedraw(hwnd);
		break;
	}
	case 'C':		// Gives the TextBox the CTRL+C feature to copy the text from the control to the clipboard
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			CopyToClipboard();
		}

		InputRedraw(hwnd);
		break;
	}
	case 'X':		// Gives the TextBox the CTRL+X feature to cut the text from the control to the clipboard
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			CopyToClipboard();
			InputDelete(hwnd, DeleteInputType::CutAndPaste);
		}

		InputRedraw(hwnd);
		break;
	}
	case VK_LEFT:	// Gives the TextBox the Navigation to Left feature by using the Left Arrow
	{
		// Doesn't let user go lower than index 0 if it's already at the beginning of the string
		if (m_CursorIndex == 0 && m_SelectIndex == 0)
		{
			break;
		}

		// SHIFT + CONTROL Press
		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
		{
			m_SelectIndex = 0;
			InputRedraw(hwnd);
			break;
		}

		// CONTROL Press
		if ((GetKeyState(VK_CONTROL) & 0x8000))
		{
			m_SelectIndex = m_CursorIndex = 0;
			InputRedraw(hwnd);
			break;
		}

		// Shift Press
		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			if (m_SelectIndex > 0)
			{
				--m_SelectIndex;
			}

			InputRedraw(hwnd);
			break;
		}

		// No Modifier Pressed
		if (m_CursorIndex == m_SelectIndex)
		{
			--m_CursorIndex;
			m_SelectIndex = m_CursorIndex;
		}
		else
		{
			if (m_SelectIndex == Text.length() || m_SelectIndex > m_CursorIndex)
			{
				m_SelectIndex = m_CursorIndex;
			}
			else
			{
				m_CursorIndex = m_SelectIndex;
			}
		}

		InputRedraw(hwnd);
		break;
	}
	case VK_RIGHT:	// Gives the TextBox the Navigation to Right feature by using the Right Arrow
	{
		// Doesn't let user get out of string if it's already at the end
		if (m_CursorIndex == Text.length() && m_SelectIndex == Text.length())
		{
			break;
		}

		// SHIFT + CONTROL Press
		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
		{
			m_SelectIndex = Text.length();
			InputRedraw(hwnd);
			break;
		}

		// CONTROL Press
		if ((GetKeyState(VK_CONTROL) & 0x8000))
		{
			m_SelectIndex = m_CursorIndex = Text.length();
			InputRedraw(hwnd);
			break;
		}

		// SHIFT Press
		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			if (m_SelectIndex < Text.length())
			{
				++m_SelectIndex;
			}

			InputRedraw(hwnd);
			break;
		}

		// No Modifier Pressed
		if (m_CursorIndex == m_SelectIndex)
		{
			++m_CursorIndex;
			m_SelectIndex = m_CursorIndex;
		}
		else
		{
			if (m_SelectIndex == 0 || m_SelectIndex < m_CursorIndex)
			{
				m_SelectIndex = m_CursorIndex;
			}
			else
			{
				m_CursorIndex = m_SelectIndex;
			}
		}

		InputRedraw(hwnd);
		break;
	}
	case VK_HOME:	// Allows the user to go to the beginning of the TextBox by pressing Home
	{
		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			m_SelectIndex = 0;
		}
		else
		{
			m_CursorIndex = m_SelectIndex = 0;
		}

		InputRedraw(hwnd);
		break;
	}
	case VK_END:	// Allows the user to go to the end of the TextBox by pressing End
	{
		if ((GetKeyState(VK_SHIFT) & 0x8000))
		{
			m_SelectIndex = Text.length();
		}
		else
		{
			m_CursorIndex = m_SelectIndex = Text.length();
		}

		InputRedraw(hwnd);
		break;
	}
	case VK_DELETE:	// Gives the TextBox the feature to remove forward keys by pressing Delete
	{
		InputDelete(hwnd, DeleteInputType::Delete);
		InputRedraw(hwnd);
		break;
	}
	case VK_BACK:	// Gives the TextBox the feature to remove backward keys by pressing Backspace
	{
		// If CTRL+Backspace is pressed, clear the whole TextBox
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			m_SelectIndex = 0;
			m_CursorIndex = Text.length();
		}

		InputDelete(hwnd, DeleteInputType::Backspace);
		InputRedraw(hwnd);
		break;
	}
	}

	WinControl::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
}

void TextBox::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	// Break processing if TextBox is multiline and ENTER is pressed
	if (!m_IsMultiline && (c == '\r' || c == '\n'))
	{
		return;
	}

	// Skip key input if TAB (Switch tabbing) or Backspace (Remove last character) is pressed
	if (c == VK_TAB || c == VK_BACK)
	{
		return;
	}

	// Block key input if CTRL key is pressed. Only CTRL+A, CTRL+C, CTRL+V and CTRL+X is allowed 
	if (0x8000 & GetKeyState(VK_CONTROL))
	{
		return;
	}

	WinControl::OnKeyPressed_Impl(hwnd, c, cRepeat);

	// Avoid key input if event is canceled
	if (ArgsOnKeyPressed.Handled)
	{
		return;
	}

	if (Text.length() < m_MaximumLenght)
	{
		if (m_CursorIndex == m_SelectIndex)
		{
			Text.insert(m_CursorIndex, 1, c);
			++m_CursorIndex;
		}
		else
		{
			size_t start = (std::min)(m_CursorIndex, m_SelectIndex);
			size_t end = (std::max)(m_CursorIndex, m_SelectIndex);
			Text.erase(start, end - start);
			Text.insert(start, 1, c);
			m_CursorIndex = start + 1;
		}

		m_SelectIndex = m_CursorIndex;
	}

	InputRedraw(hwnd);

	if (Parent != nullptr)
	{
		HandleMessageForwarder(static_cast<HWND>(Parent->Handle.ToPointer()), WM_CHAR, c, MAKELPARAM(cRepeat, 0));
	}
}

void TextBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	if (Text.length() == 0)
	{
		return;
	}

	if (x > m_CaretPosition.back())
	{
		m_CursorIndex = Text.length();
	}
	else if (x < m_CaretPosition.front())
	{
		m_CursorIndex = 0;
	}
	else
	{
		const auto idx = std::lower_bound(m_CaretPosition.begin(), m_CaretPosition.end(), x);
		m_CursorIndex = idx - m_CaretPosition.begin();
	}

	m_SelectIndex = m_CursorIndex;

	WinControl::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
}

void TextBox::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	if (IsClicking())
	{
		if (keyFlags & MK_LBUTTON)
		{
			if (x > m_CaretPosition.back())
			{
				m_SelectIndex = Text.length();
			}
			else if (x < m_CaretPosition.front())
			{
				m_SelectIndex = 0;
			}
			else
			{
				const auto idx = std::lower_bound(m_CaretPosition.begin(), m_CaretPosition.end(), x);
				m_SelectIndex = idx - m_CaretPosition.begin();
			}
		}

		Update();
		InputRedraw(hwnd);
	}

	WinControl::OnMouseMove_Impl(hwnd, x, y, keyFlags);
}

void TextBox::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
{
	// Create a solid black caret. 
	CreateCaret(hwnd, (HBITMAP)NULL, 2, Font.GetSizeInPixels());
	EnableCaret();
	InputRedraw(hwnd);

	WinControl::OnFocusEnter_Impl(hwnd, hwndOldFocus);
}

void TextBox::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	SetFocus(hwndNewFocus);
	DisableCaret();
	DestroyCaret();

	WinControl::OnFocusLeave_Impl(hwnd, hwndNewFocus);
}

void TextBox::OnPaint_Impl(HWND hWnd) noexcept
{
	PAINTSTRUCT paint;

	BeginPaint(hWnd, &paint);
	InputDraw(hWnd, paint.hdc);
	EndPaint(hWnd, &paint);
}

void TextBox::CalculateCaret(HWND hwnd, const HDC& hdc) noexcept
{
	if (Text.length() == 0)
	{
		return;
	}

	// Not the best solution for now.
	// At least it calculates using the right values
	m_CaretPosition.clear();

	if (m_CaretPosition.size() < Text.length() + 1)
	{
		m_CaretPosition.resize(Text.length() + 1, 0ul);
	}

	for (int i = 0; i <= Text.length(); ++i)
	{
		if (i > 0)
		{
			SIZE s;
			GetTextExtentPoint32(hdc, Text.substr(0, i).c_str(), i, &s);
			m_CaretPosition[i] = s.cx;
		}

		switch (BorderStyle)
		{
		case BorderStyle::None: m_CaretPosition[i] += Margin.Left; break;
		case BorderStyle::FixedSingle: m_CaretPosition[i] += Margin.Left + 1; break;
		case BorderStyle::Fixed3D: m_CaretPosition[i] += Margin.Left + 2; break;
		}
	}
}

void TextBox::CopyToClipboard() const noexcept
{
	if (OpenClipboard(static_cast<HWND>(Handle.ToPointer())))
	{
		size_t start = 0;
		size_t end = 0;

		if (m_SelectIndex < m_CursorIndex)
		{
			start = m_SelectIndex;
			end = m_CursorIndex;
		}
		else
		{
			start = m_CursorIndex;
			end = m_SelectIndex;
		}

		HGLOBAL clipbuffer;
		char* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, end - start + 1);
		if (clipbuffer > 0)
		{
			buffer = reinterpret_cast<char*>(GlobalLock(clipbuffer));
			if (buffer > 0)
			{
				strcpy_s(buffer, end - start + 1, Text.substr(start, end - start).c_str());
			}
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT, clipbuffer);
		}
		CloseClipboard();
	}
}

void TextBox::PasteFromClipboard() noexcept
{
	OpenClipboard(NULL);
	HANDLE h = GetClipboardData(CF_TEXT);

	InputDelete(static_cast<HWND>(Handle.ToPointer()), DeleteInputType::CutAndPaste);

	// Paste data from clipboard
	std::ostringstream cb;
	cb << reinterpret_cast<char*>(GlobalLock(h));

	if (cb)
	{
		Text.insert(m_CursorIndex, cb.str());

		m_CursorIndex += cb.str().length();
		m_SelectIndex = m_CursorIndex;
	}

	GlobalUnlock(h);
	CloseClipboard();
}

void TextBox::EnableCaret() noexcept
{
	if (!m_IsCaretVisible)
	{
		ShowCaret(static_cast<HWND>(Handle.ToPointer()));
		m_IsCaretVisible = true;
	}
}

void TextBox::DisableCaret() noexcept
{
	if (m_IsCaretVisible)
	{
		HideCaret(static_cast<HWND>(Handle.ToPointer()));
		m_IsCaretVisible = false;
	}
}

void TextBox::PrintDebug() const noexcept
{
	std::ostringstream oss;
	oss << "Cursor: ";
	for (int i = 0; i < m_CursorIndex; ++i) oss << "   ";
	oss << " | " << std::endl;
	oss << "Select: ";
	for (int i = 0; i < m_SelectIndex; ++i) oss << "   ";
	oss << " | " << std::endl;
	oss << "  Text: ";
	for (int i = 0; i < Text.size(); ++i) oss << " " << Text[i] << " ";
	oss << "\\0" << std::endl;
	OutputDebugString(oss.str().c_str());
}

void TextBox::InputDelete(HWND hWnd, DeleteInputType deleteType) noexcept
{
	// Will try to optimize this layer.
	// The important part now is to make it work.
	switch (deleteType)
	{
	case DeleteInputType::Backspace:
	{
		// Does nothing if backspace is pressed on the beginning of the string
		if (m_SelectIndex == 0 && m_CursorIndex == 0)
		{
			return;
		}

		if (m_SelectIndex < m_CursorIndex)
		{
			Text.erase(m_SelectIndex, GetSelectionLenght());
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, GetSelectionLenght());
			m_SelectIndex = m_CursorIndex;
		}
		else
		{
			Text.erase(m_CursorIndex - 1, 1);

			if (m_SelectIndex > 0)
			{
				--m_SelectIndex;
				--m_CursorIndex;
			}
		}

		break;
	}
	case DeleteInputType::Delete:
	{
		// Does nothing if delete is pressed on the end of the string
		if (m_SelectIndex == Text.size() && m_CursorIndex == Text.size())
		{
			return;
		}

		if (m_SelectIndex < m_CursorIndex)
		{
			Text.erase(m_SelectIndex, GetSelectionLenght());
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, GetSelectionLenght());
			m_SelectIndex = m_CursorIndex;
		}
		else
		{
			// Delete doesn't move cursor when used
			Text.erase(m_CursorIndex, 1);
		}

		break;
	}
	case DeleteInputType::CutAndPaste:
	{
		if (m_SelectIndex == m_CursorIndex)
		{
			return;
		}

		if (m_SelectIndex < m_CursorIndex)
		{
			Text.erase(m_SelectIndex, GetSelectionLenght());
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, GetSelectionLenght());
			m_SelectIndex = m_CursorIndex;
		}
		break;
	}
	}
}

void TextBox::InputRedraw(HWND hWnd) noexcept
{
	HDC hdc = GetDC(hWnd);
	InputDraw(hWnd, hdc);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hdc);
}

void TextBox::InputDraw(HWND hWnd, HDC& hdc) noexcept
{
#ifdef _DEBUG
	PrintDebug(); // Show string, cursor and selection indices on Output Window
#endif

	// Set the control size in relation to the font size
	int height = 5;
	for (int i = 5, j = 0; i < Font.GetSizeInPixels(); ++i, ++j)
	{
		if (j == 2)
		{
			height += 2;
			j = -1;
		}
		else
		{
			height += 1;
		}
	}
	// Size is always the default plus the calculated area size
	Size.Height = 9 + height;
	SetWindowPos(hWnd, HWND_TOP, Location.X, Location.Y, Size.Width, Size.Height, 0);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, Size.Width, Size.Height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

	HFONT hFont = CreateFont(Font.GetSizeInPixels(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());

	RECT r, cr;
	GetClientRect(hWnd, &cr);

	// Create TextBox border
	switch (BorderStyle)
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

		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(100, 100, 100));
		old_pen = SelectObject(hdcMem, pen);
		Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);

		// Adjust padding for text
		cr.left += 1;
		cr.top += 1;
		cr.right -= 1;
		cr.bottom -= 1;

		//Clean up
		SelectObject(hdcMem, old_pen);
		DeleteObject(old_pen);
		SelectObject(hdcMem, pen);
		DeleteObject(pen);

		break;
	}
	case BorderStyle::Fixed3D:
	{
		HPEN pen;
		HGDIOBJ old_pen;

		if (m_IsTabSelected)
		{
			// Draw outer border
			pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 120, 215));
			old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);

			// Adjust padding for next border
			cr.left += 1;
			cr.top += 1;
			cr.right -= 1;
			cr.bottom -= 1;

			// Draw inner border
			pen = CreatePen(PS_INSIDEFRAME, 1, RGB(255, 255, 255));
			old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);
		}
		else
		{
			// Draw outer border
			pen = CreatePen(PS_INSIDEFRAME, 1, RGB(122, 122, 122));
			old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);

			// Adjust padding for next border
			cr.left += 1;
			cr.top += 1;
			cr.right -= 1;
			cr.bottom -= 1;

			// Draw inner border
			pen = CreatePen(PS_INSIDEFRAME, 1, RGB(255, 255, 255));
			old_pen = SelectObject(hdcMem, pen);
			Rectangle(hdcMem, cr.left, cr.top, cr.right, cr.bottom);
		}

		//Clean up
		SelectObject(hdcMem, old_pen);
		DeleteObject(old_pen);
		SelectObject(hdcMem, pen);
		DeleteObject(pen);

		// Adjust padding for text
		cr.left += 1;
		cr.top += 1;
		cr.right -= 1;
		cr.bottom -= 1;
		break;
	}
	}

	// Draw background
	HBRUSH brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
	FillRect(hdcMem, &cr, brush);
	SelectObject(hdcMem, brush);
	DeleteObject(brush);

	// Adjust text margin after border draw
	cr.left += Margin.Left;
	cr.top += Margin.Top;
	cr.right -= Margin.Right;
	cr.bottom -= Margin.Bottom;

	// Adjust text in center based on default TextBox size with font size as 1
	int spacingForAlignment = cr.bottom - Font.GetSizeInPixels();
	cr.top += (spacingForAlignment / 2) - 4;	// 4 Pixels extra for small TextBox and underline characteres

	// BOTTOM ALIGNMENT
	//cr.top += (cr.top + Font.GetSizeInPixels()) / 2;

	CopyRect(&r, &cr);
	SelectObject(hdcMem, hFont);

	// Recalculate Caret position for each character
	CalculateCaret(hWnd, hdcMem);

	// Draw for when control is selected and cursor index is the same as select index
	if (!m_IsTabSelected || m_CursorIndex == m_SelectIndex)
	{
		SetBkMode(hdcMem, TRANSPARENT);
		SetTextColor(hdcMem, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		DrawText(hdcMem, Text.c_str(), -1, &r, DT_LEFT | DT_VCENTER);
		DrawText(hdcMem, Text.c_str(), m_CursorIndex, &r, DT_LEFT | DT_VCENTER | DT_CALCRECT);

		if (m_CursorIndex == 0)
		{
			r.right = cr.left;
		}
	}
	else
	{
		// Such beauty with a different function for draw
		PaintSelection(hdcMem, r, (std::min)(m_CursorIndex, m_SelectIndex), (std::max)(m_CursorIndex, m_SelectIndex));
	}

	// Caret position for after
	if (GetFocus() == hWnd)
	{
		std::ostringstream oss;

		if (m_CursorIndex == m_SelectIndex)
		{
			if (r.right > cr.right)
			{
				oss << "Caret X: " << cr.right << " | " << "Caret Y: " << cr.top << std::endl;
				SetCaretPos(cr.right, cr.top);
			}
			else
			{
				oss << "Caret X: " << r.right << " | " << "Caret Y: " << cr.top << std::endl;
				SetCaretPos(r.right, cr.top);
			}

			EnableCaret();
		}
		else
		{
			DisableCaret();
		}
	}

	// Perform the bit-block transfer between the memory Device Context which has the next bitmap
	// with the current image to avoid flickering
	BitBlt(hdc, 0, 0, Size.Width, Size.Height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmOld);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	SelectObject(hdcMem, hFont);
	DeleteObject(hFont);
	ReleaseDC(hWnd, hdcMem);
	DeleteDC(hdcMem);
}

void TextBox::PaintSelection(HDC& hdc, RECT& r, size_t start, size_t end) const noexcept
{
	/**************************************************************************************************/
	/* Paint selection section by section
	The logic is working flawlessly with just small pixel flaws on the first letter :D
	However, it'll be very hard to implement autoscroll with Caret and Text redraw because the redraw
	process is executed after the key press, meaning that the key would be draw one time, check it's
	position and then redraw again in the right region... GEEZZZ

	I'll try to find a proper way to implement this in a better to handle the scrolling for two reasons:
		1 - AS A DECENT TEXTBOX, IT MUST HAVE THE FEATURE;
		2 - Multiline TextBox in the future (Try no to cry, cry a lot)		:'(

	As everything is working fine except the scrolling, I'll delay this feature to the future.
	/**************************************************************************************************/
	int currentX = 0;

	// Draw text prior to selection
	SIZE s;

	if (start != 0)
	{
		GetTextExtentPoint32(hdc, Text.substr(0, start).c_str(), start, &s);
		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		TextOut(hdc, r.left, r.top, Text.substr(0, start).c_str(), start);
		currentX += s.cx;
	}

	GetTextExtentPoint32(hdc, Text.substr(start, end - start).c_str(), end - start, &s);
	SetBkMode(hdc, OPAQUE);
	SetBkColor(hdc, RGB(0, 120, 215));
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, r.left + currentX, r.top, Text.substr(start, end - start).c_str(), end - start);
	currentX += s.cx;
	r.right = currentX;

	if (end != Text.length())
	{
		GetTextExtentPoint32(hdc, Text.substr(end, Text.length() - end).c_str(), Text.length() - end, &s);
		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		TextOut(hdc, r.left + currentX, r.top, Text.substr(end, Text.length() - end).c_str(), Text.length() - end);
	}
}

TextBox::TextBox(Control* parent, int width, int x, int y)
	:
	TextBox(parent, "", width, x, y)			// Default control size without font is 9
{

}

TextBox::TextBox(Control* parent, const std::string& name, int width, int x, int y)
	:
	WinControl(parent, name, width, 9, x, y),	// Default control size without font is 9
	m_SelectIndex(Text.length()),
	m_CursorIndex(Text.length()),
	m_IsCaretVisible(false),
	m_IsMultiline(false),
	m_MaximumLenght(32767)
{
	Initialize();
}

TextBox::~TextBox()
{

}

void TextBox::Initialize()
{
	// Create window and get its handle
	Handle = CreateWindow(
		TextBoxClass::GetName(),																			// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | SS_LEFT,										// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		Size.Width,																							// Width
		Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                												// Menu handle
		TextBoxClass::GetInstance(),																		// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
	}

	// Set default TextBox margin to 3 pixels
	Margin = 3;
	m_BackgroundColor = Color::Window();
	m_ForeColor = Color::WindowText();
}

int TextBox::GetSelectionLenght() const noexcept
{
	if (m_CursorIndex < m_SelectIndex)
	{
		return m_SelectIndex - m_CursorIndex;
	}
	else
	{
		return m_CursorIndex - m_SelectIndex;
	}
}

std::string TextBox::GetSelectedText() const noexcept
{
	if (m_CursorIndex < m_SelectIndex)
	{
		return Text.substr(m_CursorIndex, GetSelectionLenght());
	}
	else
	{
		return Text.substr(m_SelectIndex, GetSelectionLenght());
	}
}

void TextBox::Hide()
{
	if (IsShown())
	{
		IsVisible = false;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_HIDE);
	}
}

void TextBox::Show()
{
	if (!IsShown())
	{
		IsVisible = true;
		ShowWindow(static_cast<HWND>(Handle.ToPointer()), SW_SHOWDEFAULT);
	}
}

unsigned int TextBox::GetMaximumLength() const noexcept
{
	return m_MaximumLenght;
}

void TextBox::SetMaximumLength(unsigned int maximumLength) noexcept
{
	// If TextBox is single line, the height is controlled by the Font size
	if (m_IsMultiline)
	{
		m_MaximumLenght = maximumLength;
	}
}