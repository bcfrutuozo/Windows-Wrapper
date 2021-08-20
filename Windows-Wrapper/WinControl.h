#pragma once

#include "Control.h"
#include "Font.h"

#include <functional>

class WinControl : public Control
{
	friend class Window;
	friend class Button;
	friend class TextBox;

private:

	bool m_Enabled;

	// Tabulation fields
	unsigned int m_TabIndex;
	static unsigned int m_IncrementalTabIndex;

	// Event fields
	EventArgs ArgsDefault;
	OnClosingEventArgs ArgsOnClosing;
	OnClosedEventArgs ArgsOnClosed;
	KeyEventArgs ArgsOnKeyDown;
	KeyPressEventArgs ArgsOnKeyPressed;
	KeyEventArgs ArgsOnKeyUp;
	MouseEventArgs ArgsOnMouseMove;
	MouseEventArgs ArgsOnMouseDown;
	MouseEventArgs ArgsOnMouseClick;
	MouseEventArgs ArgsOnMouseUp;
	MouseEventArgs ArgsOnMouseDoubleClick;
	MouseEventArgs ArgsOnMouseWheel;

	/* Global events declaration
	All are virtual to be overritten on the derived classes. Not all should be, but events like OnPaint(), OnMouseOver(),
	OnKeyPressed() have different behaviors on each type of Control. So they are all virtual to decouple each kind of 
	functionality. */
	virtual void OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized) noexcept;
	virtual void OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept;
	virtual int OnClosing_Impl(HWND hwnd) noexcept;
	virtual void OnClosed_Impl(HWND hwnd) noexcept;
	virtual HBRUSH OnColorControl_Impl(HWND hwnd, HDC hdc, HWND hwndChild, int type);
	virtual void OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) noexcept;
	virtual void OnEnable_Impl(HWND hwnd, bool fEnable);
	virtual int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept;
	virtual void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept;
	virtual void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept;
	virtual int OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept;
	virtual void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept;
	virtual void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept;
	virtual void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept;
	virtual void OnMouseLeave_Impl(HWND hwnd) noexcept;
	virtual void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys) noexcept;
	virtual void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept;
	virtual int OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys) noexcept;
	virtual void OnPaint_Impl(HWND hwnd) noexcept;
	virtual void OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput) noexcept;
	virtual int OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg) noexcept;
	virtual void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept;
	virtual void OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status) noexcept;

	WinControl* GetByTabIndex(const unsigned int& index) noexcept;

protected:

	// Variable to check if control is tab selected
	bool m_IsTabSelected;

	WinControl(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept;
	virtual ~WinControl() = default;

	// Static function which handle WinAPI messages to corresponding member function of the control
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/* This function is the savior...
	Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept;

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	WinControl* GetPreviousControl() noexcept;
	WinControl* GetNextControl() noexcept;

public:

	Font Font;

	bool IsEnabled() const noexcept;
	void Enable() noexcept;
	void Disable() noexcept;
	int GetTabIndex() const noexcept;
	void SetTabIndex(const unsigned int& index) noexcept;
	void Update() override;
};