#pragma once

#include "Control.h"

class TextBox final: public Control
{
	friend class Control;

private:

	// Used to track Caret positioning for input
	size_t m_SelectIndex;
	size_t m_CursorIndex;

	/*
	Documentation: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showcaret
	Hiding is cumulative.
	If your application calls HideCaret five times in a row, it must also call ShowCaret five times before the caret reappears.

	That's why we need to have bool to check if Caret is visible. This kind of thing really motivates me to code. Even when my
	code sucks, it might not suck as WinAPI. Geez...
	*/
	bool m_IsCaretVisible;
	bool m_IsMultiline;
	unsigned int m_MaximumLenght;
	std::vector<int> m_CaretPosition;

	enum class DeleteInputType
	{
		Backspace,
		Delete,
		CutAndPaste,
	};

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	int OnGetDLGCode_Impl(HWND hwnd, LPMSG msg) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept override;
	void OnPaint_Impl(HWND hWnd) noexcept override;

	void CalculateCaret(HWND hwnd, const HDC& hdc) noexcept;
	void CopyToClipboard() const noexcept;
	void PasteFromClipboard() noexcept;
	void EnableCaret() noexcept;
	void DisableCaret() noexcept;
	void PrintDebug() const noexcept;
	void InputDelete(HWND hWnd, DeleteInputType deleteType) noexcept;
	void InputRedraw(HWND hWnd) noexcept;
	void InputDraw(HWND hWnd, HDC& hdc) noexcept;
	void PaintSelection(HDC& hdc, RECT& r, size_t start, size_t end) const noexcept;

	TextBox(Control* parent, int width, int x, int y);
	TextBox(Control* parent, const std::string& name, int width, int x, int y);

public:

	BorderStyle BorderStyle;

	virtual ~TextBox();

	size_t GetSelectionLenght() const noexcept;
	std::string GetSelectedText() const noexcept;
	void Initialize() override;

	unsigned int GetMaximumLength() const noexcept;
	void SetMaximumLength(unsigned int maximumLength) noexcept;
};