#include "TextBox.h"

#include <algorithm>

// Singleton WndClass
TextBox::TextBoxClass TextBox::TextBoxClass::m_TextBoxClass;

// Window class declarations
TextBox::TextBoxClass::TextBoxClass() noexcept
	:
	m_Instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
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
	case VK_TAB:	// Allows the user to change controls by pressing Tab
	{
		// Forward input messages to the parent window
		if (Parent != nullptr)
		{
			const WinControl* newCtl;

			// Previous Control
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				newCtl = GetPreviousControl();
			}
			else // Next control
			{
				newCtl = GetNextControl();
			}

			if (newCtl != nullptr)
			{
				SetFocus(static_cast<HWND>(newCtl->Handle.ToPointer()));
			}
		}
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
}

void TextBox::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	if (c < VK_SPACE || c > VK_DIVIDE)
		return;

	if (Text.length() + 1 < MAXINPUTBUF)
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
	SetFocus(hwnd);
}

void TextBox::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
{
	// Set the cursor at the end when entering the control
	m_CursorIndex = Text.length();

	// Create a solid black caret. 
	CreateCaret(hwnd, (HBITMAP)NULL, 2, std::abs(Font.GetSize()));
	EnableCaret();
	InputRedraw(hwnd);
	InvalidateRect(hwnd, nullptr, true);
	m_IsTabSelected = true;
}

void TextBox::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	// Remove selection on leave
	m_CursorIndex = Text.length();
	m_SelectIndex = m_CursorIndex;
	InvalidateRect(hwnd, nullptr, true);

	SetFocus(hwndNewFocus);
	DisableCaret();
	DestroyCaret();
	InvalidateRect(hwndNewFocus, nullptr, true);
	m_IsTabSelected = false;
}

void TextBox::OnPaint_Impl(HWND hWnd) noexcept
{
	HDC dc;
	PAINTSTRUCT paint;

	dc = BeginPaint(hWnd, &paint);
	InputDraw(hWnd, dc);
	EndPaint(hWnd, &paint);
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
	DisableCaret();
	HDC hdc = GetDC(hWnd);
	InputDraw(hWnd, hdc);
	ReleaseDC(hWnd, hdc);
}

void TextBox::InputDraw(HWND hWnd, HDC hdc) noexcept
{
#ifdef _DEBUG
	PrintDebug(); // Show string, cursor and selection indices on Output Window
#endif

	SetBkMode(hdc, OPAQUE);

	HPEN pen;
	RECT r, cr;

	HFONT hFont = CreateFont(Font.GetSize(), 0, 0, 0, Font.IsBold() ? FW_BOLD : FW_NORMAL, Font.IsItalic(), Font.IsUnderline(), Font.IsStrikeOut(), ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Font.GetName().c_str());

	GetClientRect(hWnd, &cr);

	//Create pen for button border
	switch (BorderStyle)
	{
	case BorderStyle::None:
	{
		pen = CreatePen(PS_SOLID, 0, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		//Select our brush into hDC
		HGDIOBJ old_pen = SelectObject(hdc, pen);
		HGDIOBJ old_brush;

		m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		old_brush = SelectObject(hdc, m_Brush);

		Rectangle(hdc, cr.left, cr.top, cr.right, cr.bottom);

		//Clean up
		SelectObject(hdc, old_pen);
		DeleteObject(pen);
		break;
	}
	case BorderStyle::FixedSingle:
	{
		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(100, 100, 100));
		//Select our brush into hDC
		HGDIOBJ old_pen = SelectObject(hdc, pen);
		HGDIOBJ old_brush;

		m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		old_brush = SelectObject(hdc, m_Brush);
		Rectangle(hdc, cr.left, cr.top, cr.right, cr.bottom);

		//Clean up
		SelectObject(hdc, old_pen);
		SelectObject(hdc, old_brush);
		DeleteObject(pen);
		break;
	}
	case BorderStyle::Fixed3D:
	{
		pen = CreatePen(PS_INSIDEFRAME, 1, RGB(122, 122, 122));

		HGDIOBJ old_pen = SelectObject(hdc, pen);
		HGDIOBJ old_inner;

		// Draw inner border
		m_Brush = CreateSolidBrush(RGB(255, 255, 255));
		old_inner = SelectObject(hdc, m_Brush);
		Rectangle(hdc, cr.left, cr.top, cr.right, cr.bottom);

		cr.left += 1;
		cr.top += 1;
		cr.right -= 1;
		cr.bottom -= 1;

		// Draw background
		m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
		old_inner = SelectObject(hdc, m_Brush);
		Rectangle(hdc, cr.left, cr.top, cr.right, cr.bottom);

		//Clean up
		SelectObject(hdc, old_pen);
		SelectObject(hdc, old_inner);
		DeleteObject(pen);

		break;
	}
	}

	// Adjust text margin after border draw
	cr.left += Margin.Left;
	cr.top += Margin.Top;
	cr.right -= Margin.Top;
	cr.bottom -= Margin.Bottom;

	CopyRect(&r, &cr);
	SelectObject(hdc, hFont);

	if (m_CursorIndex > 0 || (m_CursorIndex == 0 && m_CursorIndex != m_SelectIndex))
	{
		if (m_CursorIndex == m_SelectIndex)
		{
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
			DrawText(hdc, Text.c_str(), -1, &r, DT_LEFT | DT_TOP);
			DrawText(hdc, Text.c_str(), m_CursorIndex, &r, DT_LEFT | DT_TOP | DT_CALCRECT);
		}
		else
		{
			int currentX = 0;

			if (m_CursorIndex > m_SelectIndex)
			{
				for (int i = 0; i < m_SelectIndex; ++i)
				{
					if (i != 0)
					{
						SIZE s;
						GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
						currentX = s.cx;
					}
					SetBkMode(hdc, TRANSPARENT);
					SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}

				for (int i = m_SelectIndex; i < m_CursorIndex; ++i)
				{
					SIZE s;
					GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
					currentX = s.cx;
					r.right = currentX;
					SetBkMode(hdc, OPAQUE);
					Color c = Color::Selection();
					SetBkColor(hdc, RGB(c.GetR(), c.GetG(), c.GetB()));
					SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}

				for (int i = m_CursorIndex; i < Text.length(); ++i)
				{
					SIZE s;
					GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
					currentX = s.cx;
					SetBkMode(hdc, TRANSPARENT);
					SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}
			}
			else
			{
				for (int i = 0; i < m_CursorIndex; ++i)
				{
					if (i != 0)
					{
						SIZE s;
						GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
						currentX = s.cx;
					}

					SetBkMode(hdc, TRANSPARENT);
					SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}

				for (int i = m_CursorIndex; i < m_SelectIndex; ++i)
				{
					SIZE s;
					GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
					currentX = s.cx;
					SetBkMode(hdc, OPAQUE);
					Color c = Color::Selection();
					SetBkColor(hdc, RGB(c.GetR(), c.GetG(), c.GetB()));
					SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}

				for (int i = m_SelectIndex; i < Text.length(); ++i)
				{
					SIZE s;
					GetTextExtentPoint32A(hdc, Text.substr(0, i).c_str(), i, &s);
					currentX = s.cx;
					SetBkMode(hdc, TRANSPARENT);
					SetBkColor(hdc, RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));
					SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
					TextOut(hdc, r.left + currentX, r.top, Text.substr(i, 1).c_str(), 1);
				}
			}
		}
	}
	else
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(m_ForeColor.GetR(), m_ForeColor.GetG(), m_ForeColor.GetB()));
		DrawText(hdc, Text.c_str(), -1, &r, DT_LEFT | DT_TOP);
		r.right = cr.left;
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

		OutputDebugString(oss.str().c_str());
	}

	DeleteObject(hFont);
}

TextBox::TextBox(Control* parent, int width, int height, int x, int y)
	:
	TextBox(parent, "", width, height, x, y)
{

}

TextBox::TextBox(Control* parent, const std::string& name, int width, int height, int x, int y)
	:
	WinControl(parent, name, width, height, x, y),
	m_SelectIndex(Text.length()),
	m_CursorIndex(Text.length()),
	m_IsCaretVisible(false)
{
	Initialize();
}

TextBox::~TextBox()
{

}

void TextBox::Initialize() noexcept
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

void TextBox::Disable() noexcept
{

}

void TextBox::Enable() noexcept
{

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