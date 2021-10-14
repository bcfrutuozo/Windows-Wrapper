#pragma once

#include "Control.h"

class TextBox final: public Control
{
	friend class Control;

private:

	// Used to track Caret positioning for input
	size_t m_SelectIndex;
	size_t m_CursorIndex;

	// Average character size for specified font
	int nCharX;
	int nCharY;

	/*
	Documentation: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showcaret
	Hiding is cumulative.
	If your application calls HideCaret five times in a row, it must also call ShowCaret five times before the caret reappears.

	That's why we need to have bool to check if Caret is visible. This kind of thing really motivates me to code. Even when my
	code sucks, it might not suck as NativeWindow. Geez...
	*/
	bool m_IsCaretVisible;

	bool m_IsMultiline;
	bool m_HasTextChanged;
	unsigned int m_MaximumLenght;
	std::vector<int> m_CaretPosition;
	HorizontalAlignment m_TextAlign;
	BorderStyle m_BorderStyle;
	int m_LinesNumber;
	bool m_IsWordWrap;

	enum class DeleteInputType
	{
		Backspace,
		Delete,
		CutAndPaste,
	};

	void PreDraw(Graphics* const graphics) override;
	void Draw(Graphics* const graphics, Drawing::Rectangle rectangle) override;
	void OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	int OnGetDLGCode_Impl(HWND hwnd, LPMSG msg) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept override;

	void CopyToClipboard() const noexcept;
	void PasteFromClipboard() noexcept;
	void EnableCaret() noexcept;
	void DisableCaret() noexcept;
	void PrintDebug() const noexcept;
	void InputDelete(DeleteInputType deleteType) noexcept;
	void InputRedraw() noexcept;
	int CalculateHeightForSingleLine() noexcept;

	TextBox(Control* parent, int width, int x, int y);
	TextBox(Control* parent, const std::string& name, int width, int x, int y);

public:
	
	virtual ~TextBox();

	size_t GetSelectionLenght() const noexcept;
	std::string GetSelectedText() const noexcept;
	void Initialize() override;

	HorizontalAlignment GetTextAlign() const noexcept;
	void SetTextAlign(HorizontalAlignment alignment) noexcept;
	BorderStyle GetBorderStyle() const noexcept;
	void SetBorderStyle(BorderStyle borderStyle) noexcept;
	unsigned int GetMaximumLength() const noexcept;
	void SetMaximumLength(unsigned int maximumLength) noexcept;
	
	bool IsMultiline() const noexcept;
	void EnableMultiline() noexcept;
	void DisableMultiline() noexcept;
	bool IsWordWrapped() const noexcept;
	void EnableWordWrap() noexcept;
	void DisableWordWrap() noexcept;
};