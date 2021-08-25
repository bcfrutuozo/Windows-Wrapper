#pragma once

#include "Component.h"
#include "IHandle.h"
#include "Event.h"
#include "Font.h"
#include "Size.h"
#include "OnClosingEventArgs.h"
#include "OnClosedEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyEventArgs.h"
#include "KeyPressEventArgs.h"

class Window;

class WinAPI : public Component, public IHandle
{
	/*
	* Friend classes to allow access between children.
	* It will be useful to implement hidden methods from the final user and at the same time,
	* improve the Interops between WinAPI controls
	*/
	friend class Control;
	friend class Button;
	friend class TextBox;
	friend class Window;
	friend class ProgressBar;
	friend class Label;
	friend class ToolStrip;

private:

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;
	bool m_IsMouseOver;
	bool m_IsClicking;
	bool m_Enabled;

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

	/***** Global events declaration *****/
	/* All are virtual to be overritten on the derived classes. Not all should be, but events like OnPaint(), OnMouseOver(),
	OnKeyPressed() have different behaviors on each type of Control. So they are all virtual to decouple each kind of
	functionality. */

	virtual void OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized) noexcept;
	virtual void OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept;
	virtual int OnClosing_Impl(HWND hwnd) noexcept;
	virtual void OnClosed_Impl(HWND hwnd) noexcept;
	virtual void OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct) noexcept;
	virtual void OnEnable_Impl(HWND hwnd, bool fEnable);
	virtual int OnEraseBackground_Impl(HWND hwnd, HDC hdc) noexcept;
	virtual void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept;
	virtual void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept;
	virtual int OnGetDLGCode(HWND hwnd, LPMSG msg) noexcept;

	/* Sent when a menu is about to become active. It occurs when the user clicks an item on the menu bar or presses
	a menu key. This allows the application to modify the menu before it is displayed. */
	virtual void OnInitMenu_Impl(HWND hwnd, HMENU hMenu) noexcept;

	/* Sent when a drop-down menu or submenu is about to become active. This allows an application to modify the menu
	before it is displayed, without changing the entire menu. */
	virtual void OnInitMenuPopup_Impl(HWND hwnd, HMENU hMenu, unsigned int item, bool fSystemMenu) noexcept;
	virtual void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept;
	virtual void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat) noexcept;
	virtual void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept;

	/* Sent when a menu is active and the user presses a key that does not correspond to any mnemonic or accelerator
	key. This message is sent to the window that owns the menu. */
	virtual int OnMenuChar_Impl(HWND hwnd, unsigned int ch, unsigned int flags, HMENU hmenu) noexcept;

	/* Sent to a menu's owner window when the user selects a menu item. */
	virtual void OnMenuSelect_Impl(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, unsigned int flags) noexcept;
	virtual void OnMouseLeave_Impl(HWND hwnd) noexcept;
	virtual void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept;
	virtual void OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys) noexcept;
	virtual void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept = 0;
	virtual int OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys) noexcept;
	virtual void OnPaint_Impl(HWND hwnd) noexcept = 0;
	virtual void OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput) noexcept;
	virtual int OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg) noexcept;
	virtual void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy) noexcept;
	virtual void OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status) noexcept;

protected:

	std::string Text;
	WinAPI* Parent;
	Size m_Size;

	WinAPI(WinAPI* parent, const std::string& text, int width, int height) noexcept;
	virtual ~WinAPI();

	// Static function which handle WinAPI messages to corresponding member function of the control
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/* This function is the savior...
	Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam) noexcept;

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	Size CalculateSizeByFont() noexcept;
	inline unsigned int GetControlDefaultHeight() const noexcept;
	unsigned int GetId() const noexcept;
	void SetMouseOverState(bool state) noexcept;
	void SetClickingState(bool state) noexcept;

public:

	Font Font;
	
	Window* GetWindow() noexcept;
	bool IsMouseOver() const noexcept;
	bool IsClicking() const noexcept;
	bool IsEnabled() const noexcept;
	void Enable() noexcept;
	void Disable() noexcept;
	virtual void Update();
};