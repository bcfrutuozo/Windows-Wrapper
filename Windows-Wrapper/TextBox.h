#pragma once

#include "Control.h"
#include "IActivable.h"
#include "IHidable.h"
#include "IWinControl.h"

#define MAXINPUTBUF 256

class TextBox : public WinControl, public IHidable, public IActivable
{
private:

	// Singleton manages registration/cleanup of window class
	class TextBoxClass
	{
	private:

		static constexpr const char* m_ClassName = "TextBox Class";
		static TextBoxClass m_TextBoxClass;
		HINSTANCE m_Instance;

		TextBoxClass() noexcept;
		~TextBoxClass() noexcept;
		TextBoxClass(const TextBoxClass&) = delete;
		TextBoxClass& operator=(const TextBoxClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	int select = 0;
	int cursor = Text.size();

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	int OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept override;
	void OnPaint_Impl(HWND hWnd) noexcept override;

	void InputWndDelete(HWND hWnd)
	{
		if (select < cursor)
		{	 
			Text.pop_back();
			//memcpy(Text.data() + cursor * sizeof(wchar_t), Text.data() + select * sizeof(wchar_t), (len - select) * sizeof(wchar_t));
			//ZeroMemory(Text.data() + (len - select + cursor) * sizeof(wchar_t), (MAXINPUTBUF - len + select - cursor) * sizeof(wchar_t));
			select = cursor;
		}
		else if (select > cursor)
		{
			Text.erase(select, cursor);
			//memcpy(Text.data() + select * sizeof(wchar_t), Text.data() + cursor * sizeof(wchar_t), (len - cursor) * sizeof(wchar_t));
			//ZeroMemory(Text.data() + (len - cursor + select) * sizeof(wchar_t), (MAXINPUTBUF - len + cursor - select) * sizeof(wchar_t));
			cursor = select;
		}
		else
		{
			select = cursor;
		}
	}

	void InputWndRedraw(HWND hWnd)
	{
		HDC hdc;

		HideCaret(hWnd);

		hdc = GetDC(hWnd);
		InputWndDraw(hWnd, hdc);
		ReleaseDC(hWnd, hdc);

		ShowCaret(hWnd);
	}

	void InputWndDraw(HWND hWnd, HDC hdc)
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


		if (cursor)
			DrawText(hdc, Text.c_str(), cursor, &r, DT_LEFT | DT_TOP | DT_CALCRECT);
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

public:

	TextBox(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~TextBox();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};