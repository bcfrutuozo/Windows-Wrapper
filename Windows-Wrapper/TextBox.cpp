//#include "TextBox.h"
//#include "Exceptions.h"
//
//#include <sstream>
//
//void TextBox::PreDraw(Graphics* const graphics)
//{
//	if (m_HasFontChanged)
//	{
//		Resize(GetSize().Width, CalculateHeightForSingleLine());
//	}
//
//	if (m_HasTextChanged)
//	{
//		// Not the best solution for now.
//		// At least it calculates using the right values
//		m_CaretPosition.clear();
//
//		if (m_CaretPosition.size() < Text.length() + 1)
//		{
//			m_CaretPosition.resize(Text.length() + 1, 0ul);
//		}
//
//		auto textSize = graphics->GetTextSize(Text, GetFont());
//
//		for (int i = 0; i <= static_cast<int>(Text.length()); ++i)
//		{
//			if (i > 0)
//			{
//				auto size = graphics->GetTextSize(Text.substr(0, i), GetFont());
//				m_CaretPosition[i] = size.Width;
//			}
//
//			switch (m_BorderStyle)
//			{
//				case BorderStyle::None:
//				{
//					switch (m_TextAlign)
//					{
//						case HorizontalAlignment::Left: m_CaretPosition[i] += m_Margin.Left; break;
//						case HorizontalAlignment::Right: m_CaretPosition[i] += GetSize().Width - m_Margin.Right - textSize.Width;
//						case HorizontalAlignment::Center: m_CaretPosition[i] += ((GetSize().Width - m_Margin.Left - m_Margin.Right) / 2) - (textSize.Width / 2); break;
//					}
//
//					break;
//				}
//				case BorderStyle::FixedSingle:
//				{
//					switch (m_TextAlign)
//					{
//						case HorizontalAlignment::Left: m_CaretPosition[i] += m_Margin.Left + 1; break;
//						case HorizontalAlignment::Right: m_CaretPosition[i] += GetSize().Width - (m_Margin.Right - 1) - textSize.Width;
//						case HorizontalAlignment::Center: m_CaretPosition[i] += ((GetSize().Width - m_Margin.Left - m_Margin.Right - 2) / 2) - (textSize.Width / 2); break;
//					}
//
//					break;
//				}
//				case BorderStyle::Fixed3D:
//				{
//					switch (m_TextAlign)
//					{
//						case HorizontalAlignment::Left: m_CaretPosition[i] += m_Margin.Left + 2; break;
//						case HorizontalAlignment::Right: m_CaretPosition[i] += GetSize().Width - (m_Margin.Right - 2) - textSize.Width;
//						case HorizontalAlignment::Center: m_CaretPosition[i] += ((GetSize().Width - m_Margin.Left - m_Margin.Right - 4) / 2) - (textSize.Width / 2); break;
//					}
//
//					break;
//				}
//			}
//		}
//
//		m_HasTextChanged = false;
//	}
//
//	Control::PreDraw(graphics);
//}
//
//void TextBox::Draw(Graphics* const graphics, Drawing::Rectangle rectangle)
//{
//#ifdef _DEBUG
//	//	PrintDebug(); // Show string, cursor and selection indices on Output Window
//#endif
//
//	auto insider = rectangle;
//
//	// Create TextBox border
//	switch (m_BorderStyle)
//	{
//		case BorderStyle::None:
//		{
//			// No border is drawn
//			break;
//		}
//		case BorderStyle::FixedSingle:
//		{
//			insider = graphics->DrawRectangle(Color(100, 100, 100), insider, 1, ChartDashStyle::Solid);
//
//			break;
//		}
//		case BorderStyle::Fixed3D:
//		{
//			if (m_IsTabSelected)
//			{
//				insider = graphics->DrawRectangle(Color(0, 120, 215), insider, 1, ChartDashStyle::Solid);
//				insider = graphics->DrawRectangle(Color(255, 255, 255), insider, 1, ChartDashStyle::Solid);
//			}
//			else
//			{
//				insider = graphics->DrawRectangle(Color(122, 122, 122), insider, 1, ChartDashStyle::Solid);
//				insider = graphics->DrawRectangle(Color(255, 255, 255), insider, 1, ChartDashStyle::Solid);
//			}
//			break;
//		}
//	}
//
//	// Draw background
//	graphics->FillRectangle(GetBackgroundColor(), insider);
//
//	// Adjust text margin after border draw
//	insider.AddMargin(m_Margin);
//
//	Drawing::Rectangle r = insider;
//
//	// Draw for when control is selected and cursor index is the same as select index
//	if (!m_IsTabSelected || m_CursorIndex == m_SelectIndex)
//	{
//		r = graphics->DrawTransparentText(Text, m_TextAlign, GetFont(), GetForeColor(), insider, m_CursorIndex);
//
//		if (m_CursorIndex == 0)
//		{
//			r.Width = insider.X;
//		}
//	}
//	else
//	{
//		int start = (std::min)(static_cast<int>(m_CursorIndex), static_cast<int>(m_SelectIndex));
//		int end = (std::max)(static_cast<int>(m_CursorIndex), static_cast<int>(m_SelectIndex));
//		// Such beauty with a different function for draw
//		//PaintSelection(hdcMem, r, (std::min)(m_CursorIndex, m_SelectIndex), (std::max)(m_CursorIndex, m_SelectIndex));
//
//		std::string temp;
//		Size tempSize(0, 0);
//		Drawing::Rectangle rc = insider;
//
//		if (start != 0)
//		{
//			temp = Text.substr(0, start);
//			tempSize = graphics->GetTextSize(temp, GetFont());
//			graphics->DrawTransparentText(temp, m_TextAlign, GetFont(), GetForeColor(), rc);
//			rc.X += tempSize.Width;
//		}
//
//		temp = Text.substr(start, end - start);
//		tempSize = graphics->GetTextSize(temp, GetFont());
//		r = graphics->DrawOpaqueText(temp, m_TextAlign, GetFont(), Color(255, 255, 255), rc, Color(0, 120, 215));
//		rc.X += tempSize.Width;
//		r.Width = rc.X;
//
//		if (end != Text.length())
//		{
//			temp = Text.substr(end, Text.length() - end);
//			tempSize = graphics->GetTextSize(temp, GetFont());
//			graphics->DrawTransparentText(temp, m_TextAlign, GetFont(), GetForeColor(), rc);
//		}
//	}
//
//	// Caret position for after
//	if (GetFocus() == static_cast<HWND>(GetHandle().ToPointer()))
//	{
//		std::ostringstream oss;
//
//		if (m_CursorIndex == m_SelectIndex)
//		{
//			EnableCaret();
//
//			if (r.Width > insider.Width)
//			{
//				oss << "Caret X: " << insider.Width << " | " << "Caret Y: " << insider.Y << std::endl;
//				SetCaretPos(insider.Width, insider.Y + 2);
//			}
//			else
//			{
//				oss << "Caret X: " << r.Width << " | " << "Caret Y: " << insider.Y << std::endl;
//				SetCaretPos(r.Width, insider.Y + 2);
//			}
//		}
//		else
//		{
//			DisableCaret();
//		}
//
//		printf_s(oss.str().c_str());
//	}
//}
//
//void TextBox::OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
//{
//	// Call base function to create Graphics handle
//	Control::OnCreate_Impl(hwnd, lpCreateStruct);
//	Resize(GetSize().Width, CalculateHeightForSingleLine());
//}
//
//int TextBox::OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept
//{
//	return 1;   // Returns 1 to avoid flickering
//}
//
//int TextBox::OnGetDLGCode_Impl(HWND hwnd, LPMSG msg) noexcept
//{
//	return DLGC_WANTALLKEYS | DLGC_WANTARROWS;
//}
//
//void TextBox::OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept
//{
//	/**************************************************************************************************/
//	/* This is the most important function of the TextBox class. It's responsible to manage the user
//	input between the cursor and selection. By doing it so, it manages all navigation inside the TextBox
//	while handles and stores the indices for common delete operations, using Backspace and Delete keys.
//
//	To understand how selection and cursor index works:
//
//		OnEnter
//
//		Cursor:		        ↓
//		Select:		        ↓
//		Index:		0 1 2 3 4 5 6 7
//		Text:		T e x t B o x \0
//
//		OnLeftKeyPress (WITHOUT SHIFT)
//
//		Cursor:		      ↓					Can't go lesser than 0
//		Select:		      ↓					Can't go lesser than 0
//		Index:		0 1 2 3 4 5 6 7
//		Text:		T e x t B o x \0
//
//		OnLeftKeyPress (WITH SHIFT)
//
//		Cursor:		    ↓					Can't go lesser than 0
//		Select:		        ↓
//		Index:		0 1 2 3 4 5 6 7
//		Text:		T e x t B o x \0
//
//		OnRightKeyPress (WITHOUT SHIFT)
//
//		Cursor:		          ↓				Can't go higher than the string.length() = '\0'
//		Select:		          ↓				Can't go higher than the string.length() = '\0'
//		Index:		0 1 2 3 4 5 6 7
//		Text:		T e x t B o x \0
//
//		OnLeftKeyPress (WITH SHIFT)
//
//		Cursor:		            ↓			Can't go higher than the string.length() = '\0'
//		Select:		        ↓
//		Index:		0 1 2 3 4 5 6 7
//		Text:		T e x t B o x \0
//
//	/**************************************************************************************************/
//
//	switch (vk)
//	{
//		case 'A':		// Gives the TextBox the CTRL+A feature to select all text from the control
//		{
//			if (0x8000 & GetKeyState(VK_CONTROL))
//			{
//				m_SelectIndex = 0;
//				m_CursorIndex = Text.length();
//			}
//
//			InputRedraw();
//			break;
//		}
//		case 'V':		// Gives the TextBox the CTRL+V feature to paste the data from the clipboard to the control
//		{
//			if (0x8000 & GetKeyState(VK_CONTROL))
//			{
//				PasteFromClipboard();
//			}
//
//			InputRedraw();
//			break;
//		}
//		case 'C':		// Gives the TextBox the CTRL+C feature to copy the text from the control to the clipboard
//		{
//			if (0x8000 & GetKeyState(VK_CONTROL))
//			{
//				CopyToClipboard();
//			}
//
//			InputRedraw();
//			break;
//		}
//		case 'X':		// Gives the TextBox the CTRL+X feature to cut the text from the control to the clipboard
//		{
//			if (0x8000 & GetKeyState(VK_CONTROL))
//			{
//				CopyToClipboard();
//				InputDelete(DeleteInputType::CutAndPaste);
//			}
//
//			InputRedraw();
//			break;
//		}
//		case VK_LEFT:	// Gives the TextBox the Navigation to Left feature by using the Left Arrow
//		{
//			// Doesn't let user go lower than index 0 if it's already at the beginning of the string
//			if (m_CursorIndex == 0 && m_SelectIndex == 0)
//			{
//				break;
//			}
//
//			// SHIFT + CONTROL Press
//			if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				m_SelectIndex = 0;
//				InputRedraw();
//				break;
//			}
//
//			// CONTROL Press
//			if ((GetKeyState(VK_CONTROL) & 0x8000))
//			{
//				m_SelectIndex = m_CursorIndex = 0;
//				InputRedraw();
//				break;
//			}
//
//			// Shift Press
//			if ((GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				if (m_SelectIndex > 0)
//				{
//					--m_SelectIndex;
//				}
//
//				InputRedraw();
//				break;
//			}
//
//			// No Modifier Pressed
//			if (m_CursorIndex == m_SelectIndex)
//			{
//				--m_CursorIndex;
//				m_SelectIndex = m_CursorIndex;
//			}
//			else
//			{
//				if (m_SelectIndex == Text.length() || m_SelectIndex > m_CursorIndex)
//				{
//					m_SelectIndex = m_CursorIndex;
//				}
//				else
//				{
//					m_CursorIndex = m_SelectIndex;
//				}
//			}
//
//			InputRedraw();
//			break;
//		}
//		case VK_RIGHT:	// Gives the TextBox the Navigation to Right feature by using the Right Arrow
//		{
//			// Doesn't let user get out of string if it's already at the end
//			if (m_CursorIndex == Text.length() && m_SelectIndex == Text.length())
//			{
//				break;
//			}
//
//			// SHIFT + CONTROL Press
//			if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				m_SelectIndex = Text.length();
//				InputRedraw();
//				break;
//			}
//
//			// CONTROL Press
//			if ((GetKeyState(VK_CONTROL) & 0x8000))
//			{
//				m_SelectIndex = m_CursorIndex = Text.length();
//				InputRedraw();
//				break;
//			}
//
//			// SHIFT Press
//			if ((GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				if (m_SelectIndex < Text.length())
//				{
//					++m_SelectIndex;
//				}
//
//				InputRedraw();
//				break;
//			}
//
//			// No Modifier Pressed
//			if (m_CursorIndex == m_SelectIndex)
//			{
//				++m_CursorIndex;
//				m_SelectIndex = m_CursorIndex;
//			}
//			else
//			{
//				if (m_SelectIndex == 0 || m_SelectIndex < m_CursorIndex)
//				{
//					m_SelectIndex = m_CursorIndex;
//				}
//				else
//				{
//					m_CursorIndex = m_SelectIndex;
//				}
//			}
//
//			InputRedraw();
//			break;
//		}
//		case VK_HOME:	// Allows the user to go to the beginning of the TextBox by pressing Home
//		{
//			if ((GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				m_SelectIndex = 0;
//			}
//			else
//			{
//				m_CursorIndex = m_SelectIndex = 0;
//			}
//
//			InputRedraw();
//			break;
//		}
//		case VK_END:	// Allows the user to go to the end of the TextBox by pressing End
//		{
//			if ((GetKeyState(VK_SHIFT) & 0x8000))
//			{
//				m_SelectIndex = Text.length();
//			}
//			else
//			{
//				m_CursorIndex = m_SelectIndex = Text.length();
//			}
//
//			InputRedraw();
//			break;
//		}
//		case VK_DELETE:	// Gives the TextBox the feature to remove forward keys by pressing Delete
//		{
//			InputDelete(DeleteInputType::Delete);
//			InputRedraw();
//			break;
//		}
//		case VK_BACK:	// Gives the TextBox the feature to remove backward keys by pressing Backspace
//		{
//			// If CTRL+Backspace is pressed, clear the whole TextBox
//			if (0x8000 & GetKeyState(VK_CONTROL))
//			{
//				m_SelectIndex = 0;
//				m_CursorIndex = Text.length();
//			}
//
//			InputDelete(DeleteInputType::Backspace);
//			InputRedraw();
//			break;
//		}
//	}
//
//	Control::OnKeyDown_Impl(hwnd, vk, cRepeat, flags);
//}
//
//void TextBox::OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept
//{
//	// Break processing if TextBox is multiline and ENTER is pressed
//	if (!m_IsMultiline && (c == '\r' || c == '\n'))
//	{
//		return;
//	}
//
//	// Skip key input if TAB (Switch tabbing) or Backspace (Remove last character) is pressed
//	if (c == VK_TAB || c == VK_BACK || c == VK_ESCAPE)
//	{
//		return;
//	}
//
//	// Block key input if CTRL key is pressed. Only CTRL+A, CTRL+C, CTRL+V and CTRL+X is allowed 
//	if (0x8000 & GetKeyState(VK_CONTROL))
//	{
//		return;
//	}
//
//	Control::OnKeyPressed_Impl(hwnd, c, cRepeat);
//
//	// Avoid key input if event is canceled
//	if (ArgsOnKeyPressed.Handled)
//	{
//		return;
//	}
//
//	if (Text.length() < m_MaximumLenght)
//	{
//		if (m_CursorIndex == m_SelectIndex)
//		{
//			Text.insert(m_CursorIndex, 1, c);
//			++m_CursorIndex;
//		}
//		else
//		{
//			size_t start = (std::min)(m_CursorIndex, m_SelectIndex);
//			size_t end = (std::max)(m_CursorIndex, m_SelectIndex);
//			Text.erase(start, end - start);
//			Text.insert(start, 1, c);
//			m_CursorIndex = start + 1;
//		}
//
//		m_SelectIndex = m_CursorIndex;
//	}
//
//	InputRedraw();
//
//	if (Parent != nullptr)
//	{
//		//NativeWindow::HandleMessageForwarder(static_cast<HWND>(Parent->Handle.ToPointer()), WM_CHAR, c, MAKELPARAM(cRepeat, 0));
//	}
//}
//
//void TextBox::OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
//{
//	if (x > m_CaretPosition.back())
//	{
//		m_CursorIndex = Text.length();
//	}
//	else if (x < m_CaretPosition.front())
//	{
//		m_CursorIndex = 0;
//	}
//	else
//	{
//		const auto idx = std::lower_bound(m_CaretPosition.begin(), m_CaretPosition.end(), x);
//		m_CursorIndex = idx - m_CaretPosition.begin();
//	}
//
//	m_SelectIndex = m_CursorIndex;
//
//	Control::OnMouseLeftDown_Impl(hwnd, x, y, keyFlags);
//}
//
//void TextBox::OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept
//{
//	if (IsClicking())
//	{
//		if (keyFlags & MK_LBUTTON)
//		{
//			if (x > m_CaretPosition.back())
//			{
//				m_SelectIndex = Text.length();
//			}
//			else if (x < m_CaretPosition.front())
//			{
//				m_SelectIndex = 0;
//			}
//			else
//			{
//				const auto idx = std::lower_bound(m_CaretPosition.begin(), m_CaretPosition.end(), x);
//				m_SelectIndex = idx - m_CaretPosition.begin();
//			}
//		}
//
//		Update();
//		InputRedraw();
//	}
//
//	Control::OnMouseMove_Impl(hwnd, x, y, keyFlags);
//}
//
//void TextBox::OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept
//{
//	// Create a solid black caret. 
//	CreateCaret(hwnd, (HBITMAP)NULL, 2, nCharY);
//	EnableCaret();
//	InputRedraw();
//
//	Control::OnFocusEnter_Impl(hwnd, hwndOldFocus);
//}
//
//void TextBox::OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept
//{
//	SetFocus(hwndNewFocus);
//	DisableCaret();
//	DestroyCaret();
//
//	Control::OnFocusLeave_Impl(hwnd, hwndNewFocus);
//}
//
//void TextBox::CopyToClipboard() noexcept
//{
//	if (OpenClipboard(static_cast<HWND>(GetHandle().ToPointer())))
//	{
//		size_t start = 0;
//		size_t end = 0;
//
//		if (m_SelectIndex < m_CursorIndex)
//		{
//			start = m_SelectIndex;
//			end = m_CursorIndex;
//		}
//		else
//		{
//			start = m_CursorIndex;
//			end = m_SelectIndex;
//		}
//
//		HGLOBAL clipbuffer = NULL;
//		char* buffer = NULL;
//		EmptyClipboard();
//		clipbuffer = GlobalAlloc(GMEM_DDESHARE, end - start + 1);
//		if (clipbuffer != NULL)
//		{
//			buffer = reinterpret_cast<char*>(GlobalLock(clipbuffer));
//			if (buffer != NULL)
//			{
//				strcpy_s(buffer, end - start + 1, Text.substr(start, end - start).c_str());
//			}
//			GlobalUnlock(clipbuffer);
//			SetClipboardData(CF_TEXT, clipbuffer);
//		}
//		CloseClipboard();
//	}
//}
//
//void TextBox::PasteFromClipboard() noexcept
//{
//	OpenClipboard(NULL);
//	HANDLE h = GetClipboardData(CF_TEXT);
//
//	InputDelete(DeleteInputType::CutAndPaste);
//
//	// Paste data from clipboard
//	std::ostringstream cb;
//	cb << reinterpret_cast<char*>(GlobalLock(h));
//
//	if (cb)
//	{
//		Text.insert(m_CursorIndex, cb.str());
//
//		m_CursorIndex += cb.str().length();
//		m_SelectIndex = m_CursorIndex;
//	}
//
//	GlobalUnlock(h);
//	CloseClipboard();
//}
//
//void TextBox::EnableCaret() noexcept
//{
//	if (!m_IsCaretVisible)
//	{
//		ShowCaret(static_cast<HWND>(GetHandle().ToPointer()));
//		m_IsCaretVisible = true;
//	}
//}
//
//void TextBox::DisableCaret() noexcept
//{
//	if (m_IsCaretVisible)
//	{
//		HideCaret(static_cast<HWND>(GetHandle().ToPointer()));
//		m_IsCaretVisible = false;
//	}
//}
//
//void TextBox::PrintDebug() const noexcept
//{
//	std::ostringstream oss;
//	oss << "Cursor: ";
//	for (int i = 0; i < static_cast<int>(m_CursorIndex); ++i) oss << "   ";
//	oss << " | " << std::endl;
//	oss << "Select: ";
//	for (int i = 0; i < static_cast<int>(m_SelectIndex); ++i) oss << "   ";
//	oss << " | " << std::endl;
//	oss << "  Text: ";
//	for (int i = 0; i < static_cast<int>(Text.size()); ++i) oss << " " << Text[i] << " ";
//	oss << "\\0" << std::endl;
//	printf_s(oss.str().c_str());
//}
//
//void TextBox::InputDelete(DeleteInputType deleteType) noexcept
//{
//	// Will try to optimize this layer.
//	// The important part now is to make it work.
//	switch (deleteType)
//	{
//		case DeleteInputType::Backspace:
//		{
//			// Does nothing if backspace is pressed on the beginning of the string
//			if (m_SelectIndex == 0 && m_CursorIndex == 0)
//			{
//				return;
//			}
//
//			if (m_SelectIndex < m_CursorIndex)
//			{
//				Text.erase(m_SelectIndex, GetSelectionLenght());
//				m_CursorIndex = m_SelectIndex;
//			}
//			else if (m_SelectIndex > m_CursorIndex)
//			{
//				Text.erase(m_CursorIndex, GetSelectionLenght());
//				m_SelectIndex = m_CursorIndex;
//			}
//			else
//			{
//				Text.erase(m_CursorIndex - 1, 1);
//
//				if (m_SelectIndex > 0)
//				{
//					--m_SelectIndex;
//					--m_CursorIndex;
//				}
//			}
//
//			break;
//		}
//		case DeleteInputType::Delete:
//		{
//			// Does nothing if delete is pressed on the end of the string
//			if (m_SelectIndex == Text.size() && m_CursorIndex == Text.size())
//			{
//				return;
//			}
//
//			if (m_SelectIndex < m_CursorIndex)
//			{
//				Text.erase(m_SelectIndex, GetSelectionLenght());
//				m_CursorIndex = m_SelectIndex;
//			}
//			else if (m_SelectIndex > m_CursorIndex)
//			{
//				Text.erase(m_CursorIndex, GetSelectionLenght());
//				m_SelectIndex = m_CursorIndex;
//			}
//			else
//			{
//				// Delete doesn't move cursor when used
//				Text.erase(m_CursorIndex, 1);
//			}
//
//			break;
//		}
//		case DeleteInputType::CutAndPaste:
//		{
//			if (m_SelectIndex == m_CursorIndex)
//			{
//				return;
//			}
//
//			if (m_SelectIndex < m_CursorIndex)
//			{
//				Text.erase(m_SelectIndex, GetSelectionLenght());
//				m_CursorIndex = m_SelectIndex;
//			}
//			else if (m_SelectIndex > m_CursorIndex)
//			{
//				Text.erase(m_CursorIndex, GetSelectionLenght());
//				m_SelectIndex = m_CursorIndex;
//			}
//			break;
//		}
//	}
//}
//
//void TextBox::InputRedraw() noexcept
//{
//	m_HasTextChanged = true;
//	Update();
//}
//
//int TextBox::CalculateHeightForSingleLine() noexcept
//{
//	Size r(GetSize());
//
//	auto size = m_Graphics->GetTextSize(Text, GetFont()).Height;
//	
//	switch (m_BorderStyle)
//	{
//		case BorderStyle::None: break;
//		case BorderStyle::FixedSingle: size += 2; break;
//		case BorderStyle::Fixed3D: size += 4; break;
//	}
//
//	TEXTMETRIC tm;
//	GetTextMetrics(static_cast<HDC>(m_Graphics->GetHDC().ToPointer()), &tm);
//	nCharX = tm.tmAveCharWidth;
//	nCharY = tm.tmHeight;
//
//	return size + 8;
//}
//
//TextBox::TextBox(Control* parent, int width, int x, int y)
//	:
//	TextBox(parent, "", width, x, y)
//{
//
//}
//
//TextBox::TextBox(Control* parent, const std::string& name, int width, int x, int y)
//	:
//	Control(parent, name, width, 0, x, y),	// Default control size without font is 9
//	m_SelectIndex(Text.length()),
//	m_CursorIndex(Text.length()),
//	m_IsCaretVisible(false),
//	m_IsMultiline(false),
//	m_MaximumLenght(32767),
//	m_BorderStyle(BorderStyle::Fixed3D),
//	m_TextAlign(HorizontalAlignment::Left),
//	m_HasTextChanged(true),
//	m_IsWordWrap(false)
//{
//	//Initialize();
//}
//
//TextBox::~TextBox()
//{
//
//}
//
////void TextBox::Initialize()
////{
////	// Create window and get its handle
////	//CreateWindow(
////	//	WindowClass::GetName(),
////	//	Text.c_str(),
////	//	WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS | SS_LEFT,
////	//	m_Location.X,
////	//	m_Location.Y,
////	//	m_Size.Width,
////	//	m_Size.Height,
////	//	static_cast<HWND>(Parent->Handle.ToPointer()),
////	//	nullptr,
////	//	WindowClass::GetInstance(),
////	//	this
////	//);
////
////	//if (Handle.IsNull())
////	//{
////	//	throw CTL_LAST_EXCEPT();
////	//}
////}
//
//HorizontalAlignment TextBox::GetTextAlign() const noexcept
//{
//	return m_TextAlign;
//}
//
//void TextBox::SetTextAlign(HorizontalAlignment alignment) noexcept
//{
//	m_TextAlign = alignment;
//	m_HasTextChanged = true;
//}
//
//BorderStyle TextBox::GetBorderStyle() const noexcept
//{
//	return m_BorderStyle;
//}
//
//void TextBox::SetBorderStyle(BorderStyle borderStyle) noexcept
//{
//	m_BorderStyle = borderStyle;
//	m_HasTextChanged = true;
//}
//
//size_t TextBox::GetSelectionLenght() const noexcept
//{
//	if (m_CursorIndex < m_SelectIndex)
//	{
//		return m_SelectIndex - m_CursorIndex;
//	}
//	else
//	{
//		return m_CursorIndex - m_SelectIndex;
//	}
//}
//
//std::string TextBox::GetSelectedText() const noexcept
//{
//	if (m_CursorIndex < m_SelectIndex)
//	{
//		return Text.substr(m_CursorIndex, GetSelectionLenght());
//	}
//	else
//	{
//		return Text.substr(m_SelectIndex, GetSelectionLenght());
//	}
//}
//
//unsigned int TextBox::GetMaximumLength() const noexcept
//{
//	return m_MaximumLenght;
//}
//
//void TextBox::SetMaximumLength(unsigned int maximumLength) noexcept
//{
//	// If TextBox is single line, the height is controlled by the Font size
//	if (m_IsMultiline)
//	{
//		m_MaximumLenght = maximumLength;
//	}
//}
//
//bool TextBox::IsMultiline() const noexcept
//{
//	return m_IsMultiline;
//}
//
//void TextBox::EnableMultiline() noexcept
//{
//	m_IsMultiline = true;
//}
//
//void TextBox::DisableMultiline() noexcept
//{
//	m_IsMultiline = false;
//	Resize(GetSize().Width, CalculateHeightForSingleLine());
//}
//
//bool TextBox::IsWordWrapped() const noexcept
//{
//	return m_IsWordWrap;
//}
//
//void TextBox::EnableWordWrap() noexcept
//{
//	m_IsWordWrap = true;
//}
//
//void TextBox::DisableWordWrap() noexcept
//{
//	m_IsWordWrap = false;
//}
