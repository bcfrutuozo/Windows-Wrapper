#include "TextBox.h"

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
			InputRedraw(hwnd);

#if _DEBUG
			PrintDebug();
#endif
			break;
		}
		break;
	}
	case 'V':		// Gives the TextBox the CTRL+C feature to copy the text from the control to the clipboard
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			OpenClipboard(NULL);
			HANDLE h = GetClipboardData(CF_TEXT);

			InputDelete(hwnd, DeleteInputType::Delete);

			// Paste data from clipboard
			std::ostringstream cb;
			cb << reinterpret_cast<char*>(GlobalLock(h));

			if (cb)
			{
				Text.append(cb.str());
			}

			GlobalUnlock(h);
			CloseClipboard();
			InputRedraw(hwnd);

#if _DEBUG
			PrintDebug();
#endif
		}
		break;
	}
	case 'C':		// Gives the TextBox the CTRL+V feature to paste the data from the clipboard to the control
	{
		if (0x8000 & GetKeyState(VK_CONTROL))
		{
			if (OpenClipboard(hwnd))
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
				buffer = reinterpret_cast<char*>(GlobalLock(clipbuffer));
				strcpy_s(buffer, end - start + 1, Text.substr(start, end - start).c_str());
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT, clipbuffer);
				CloseClipboard();
			}
		}

		break;
	}
	case VK_RIGHT:	// Gives the TextBox the Navigation to Right feature by using the Right Arrow
	{
		if (m_CursorIndex >= MAXINPUTBUF || m_CursorIndex == Text.length())
			break;

		m_CursorIndex++;

		if (!(GetKeyState(VK_SHIFT) & 0x8000))
			m_SelectIndex = m_CursorIndex;

		InputRedraw(hwnd);

#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	case VK_TAB:	// Allows the user to change controls by pressing Tab
	{
		PostMessageW(GetParent(hwnd), WM_NEXTDLGCTL, GetKeyState(VK_SHIFT) & 0x8000, FALSE);
		break;
	}
	case VK_LEFT:	// Gives the TextBox the Navigation to Left feature by using the Left Arrow
	{
		if (m_CursorIndex == 0)
			break;

		m_CursorIndex--;

		if (!(GetKeyState(VK_SHIFT) & 0x8000))
			m_SelectIndex = m_CursorIndex;

		InputRedraw(hwnd);

#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	case VK_HOME:	// Allows the user to go to the beginning of the TextBox by pressing Home
	{
		m_CursorIndex = 0;

		if (!(GetKeyState(VK_SHIFT) & 0x8000))
			m_SelectIndex = m_CursorIndex;

		InputRedraw(hwnd);
#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	case VK_END:	// Allows the user to go to the end of the TextBox by pressing End
	{
		m_CursorIndex = Text.length();

		if (!(GetKeyState(VK_SHIFT) & 0x8000))
			m_SelectIndex = m_CursorIndex;

		InputRedraw(hwnd);
#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	case VK_DELETE:	// Gives the TextBox the feature to remove forward keys by pressing Delete
	{
		InputDelete(hwnd, DeleteInputType::Delete);
		InputRedraw(hwnd);
#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	case VK_BACK:	// Gives the TextBox the feature to remove backward keys by pressing Backspace
	{
		InputDelete(hwnd, DeleteInputType::Backspace);
		InputRedraw(hwnd);
#if _DEBUG
		PrintDebug();
#endif
		break;
	}
	}

	// Forward input messages to the parent window
	if (Parent != nullptr)
	{
		HandleMessageForwarder(static_cast<HWND>(Parent->Handle.ToPointer()), WM_KEYDOWN, vk, MAKELPARAM(cRepeat, flags));
	}
}

void TextBox::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
{
	if (c < VK_SPACE)
		return;

	if (Text.length() + 1 < MAXINPUTBUF)
	{
		Text.insert(m_CursorIndex, 1, c);
		++m_CursorIndex;
		m_SelectIndex = m_CursorIndex;
	}

	InputRedraw(hwnd);

#if _DEBUG
	PrintDebug();
#endif

	if (Parent != nullptr)
	{
		HandleMessageForwarder(static_cast<HWND>(Parent->Handle.ToPointer()), WM_CHAR, c, MAKELPARAM(cRepeat, 0));
	}
}

void TextBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
{
	SetFocus(hwnd);
	PostMessageW(GetParent(hwnd), WM_NEXTDLGCTL, (WPARAM)hwnd, TRUE);

	if (Parent != nullptr)
	{
		HandleMessageForwarder(static_cast<HWND>(Parent->Handle.ToPointer()), WM_LBUTTONDOWN, (WPARAM)hwnd, TRUE);
	}
}

void TextBox::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
{
	RECT r;
	GetClientRect(hwnd, &r);
	// Create a solid black caret. 
	CreateCaret(hwnd, (HBITMAP)NULL, 2, r.bottom - r.top);

	ShowCaret(hwnd);
	InputRedraw(hwnd);
}

void TextBox::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
{
	HideCaret(hwnd);
	DestroyCaret();
}

void TextBox::OnPaint_Impl(HWND hWnd) noexcept
{
	HDC dc;
	PAINTSTRUCT paint;

	dc = BeginPaint(hWnd, &paint);
	InputDraw(hWnd, dc);
	EndPaint(hWnd, &paint);
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
			Text.erase(m_SelectIndex, m_CursorIndex - m_SelectIndex);
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, m_SelectIndex - m_CursorIndex);
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
			Text.erase(m_SelectIndex, m_CursorIndex - m_SelectIndex);
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, m_SelectIndex - m_CursorIndex);
			m_SelectIndex = m_CursorIndex;
		}
		else
		{
			// Delete doesn't move cursor when used
			Text.erase(m_CursorIndex, 1);
		}

		break;
	}
	case DeleteInputType::Paste:
	{
		if (m_SelectIndex < m_CursorIndex)
		{
			Text.erase(m_SelectIndex, m_CursorIndex - m_SelectIndex);
			m_CursorIndex = m_SelectIndex;
		}
		else if (m_SelectIndex > m_CursorIndex)
		{
			Text.erase(m_CursorIndex, m_SelectIndex - m_CursorIndex);
			m_SelectIndex = m_CursorIndex;
		}
		break;
	}
	}
}

void TextBox::InputRedraw(HWND hWnd) noexcept
{
	HDC hdc;

	HideCaret(hWnd);

	hdc = GetDC(hWnd);
	InputDraw(hWnd, hdc);
	ReleaseDC(hWnd, hdc);

	ShowCaret(hWnd);
}

void TextBox::InputDraw(HWND hWnd, HDC hdc) noexcept
{
	RECT r, cr;

	GetClientRect(hWnd, &cr);

	//Create pen for button border
	HPEN  pen = CreatePen(PS_INSIDEFRAME, 1, RGB(0, 0, 0));

	m_Brush = CreateSolidBrush(RGB(m_BackgroundColor.GetR(), m_BackgroundColor.GetG(), m_BackgroundColor.GetB()));

	//Select our brush into hDC
	HGDIOBJ old_pen = SelectObject(hdc, pen);
	HGDIOBJ old_brush = SelectObject(hdc, m_Brush);

	// GOING TO THINK A WAY OF IMPLEMENT NORMAL AND ROUNDED BORDER BUTTON

	//If you want rounded button, then use this, otherwise use FillRect().

	//RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);
	Rectangle(hdc, cr.left, cr.top, cr.right, cr.bottom);
	//FillRect(hdc, &rc, m_Brush);

	//Clean up
	SelectObject(hdc, old_pen);
	SelectObject(hdc, old_brush);
	DeleteObject(pen);

	CopyRect(&r, &cr);
	DrawText(hdc, Text.c_str(), -1, &r, DT_LEFT | DT_TOP);


	if (m_CursorIndex)
		DrawText(hdc, Text.c_str(), m_CursorIndex, &r, DT_LEFT | DT_TOP | DT_CALCRECT);
	else
		r.right = cr.left;

	if (GetFocus() == hWnd)
	{
		if (r.right > cr.right)
			SetCaretPos(cr.right, cr.top);
		else
			SetCaretPos(r.right, cr.top);
	}
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
	m_CursorIndex(Text.length())
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
		TextBoxClass::GetName(),																								// Class name
		Text.c_str(),																						// Window title
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | SS_LEFT,												// Style values
		Location.X,																							// X position
		Location.Y,																							// Y position
		Size.Width,																							// Width
		Size.Height,																						// Height
		static_cast<HWND>(Parent->Handle.ToPointer()),														// Parent handle
		(HMENU)GetId(),						                												// Menu handle
		TextBoxClass::GetInstance(),																			// Module instance handle
		this																								// Pointer to the button instance to work along with HandleMessageSetup function.
	);

	if (Handle.IsNull())
	{
		throw CTL_LAST_EXCEPT();
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