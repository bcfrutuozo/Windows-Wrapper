#pragma once

#include "Control.h"
#include "IActivable.h"
#include "IHidable.h"
#include "IWinControl.h"

#define MAXINPUTBUF 256

class TextBox : public WinControl, public IHidable, public IActivable
{
private:

	// Used to track Caret positioning for input
	size_t m_SelectIndex;
	size_t m_CursorIndex;

	enum class DeleteInputType
	{
		Backspace,
		Delete,
		Paste,
	};

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

	int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept override;
	int OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept override;
	void OnPaint_Impl(HWND hWnd) noexcept override;

	void PrintDebug() const noexcept;
	void InputDelete(HWND hWnd, DeleteInputType deleteType) noexcept;
	void InputRedraw(HWND hWnd) noexcept;
	void InputDraw(HWND hWnd, HDC hdc) noexcept;

public:

	TextBox(Control* parent, int width, int height, int x, int y);
	TextBox(Control* parent, const std::string& name, int width, int height, int x, int y);
	virtual ~TextBox();

	void Disable() noexcept override;
	void Enable() noexcept override;
	void Hide() override;
	void Initialize() noexcept override;
	void Show() override;
};