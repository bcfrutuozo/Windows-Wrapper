#pragma once

#include "Exceptions.h"
#include "WindowClass.h"
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
#include "MessageMapper.h"

class Window;

class WinAPI : public Component, public IHandle
{
	/*
	* Friend classes to allow access between children.
	* It will be useful to implement hidden methods from the final user and at the same time,
	* improve the Interops between WinAPI controls
	*/
	friend class WindowClass;
	friend class Control;
	friend class Button;
	friend class TextBox;
	friend class Window;
	friend class ProgressBar;
	friend class Label;
	friend class ToolStrip;
	friend class ListControl;
	friend class ComboBox;
	friend class ListBox;
	friend class ScrollBar;
	friend class HorizontalScrollBar;
	friend class VerticalScrollBar;

private:

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;
	bool m_IsMouseOver;
	bool m_IsClicking;
	bool m_Enabled;
	static IntPtr m_OpenedControl;

	// Scrolling
	int m_HorizontalScrolling;
	int m_VerticalScrolling;
	int m_VerticalScrollingUnit;
	int m_VerticalScrollPaging;

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

	virtual void OnActivate_Impl(HWND hwnd, unsigned int state, HWND hwndActDeact, bool minimized);
	virtual void OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify);
	virtual int OnClosing_Impl(HWND hwnd);
	virtual void OnClosed_Impl(HWND hwnd);
	virtual void OnCreate_Impl(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	virtual void OnEnable_Impl(HWND hwnd, bool fEnable);
	virtual int OnEraseBackground_Impl(HWND hwnd, HDC hdc);
	virtual void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus);
	virtual void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus);
	virtual int OnGetDLGCode_Impl(HWND hwnd, LPMSG msg);
	virtual void OnHorizontalScrolling_Impl(HWND hwnd, HWND hwndCtl, unsigned int code, int pos);
	virtual void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags);
	virtual void OnKeyPressed_Impl(HWND hwnd, char c, int cRepeat);
	virtual void OnKeyUp_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags);
	virtual void OnMouseLeave_Impl(HWND hwnd);
	virtual void OnMouseMove_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseLeftUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseRightDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseRightUp_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseLeftDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseRightDoubleClick_Impl(HWND hwnd, int x, int y, unsigned int keyFlags);
	virtual void OnMouseWheel_Impl(HWND hwnd, int x, int y, int delta, unsigned int fwKeys);
	virtual void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) = 0;
	virtual int OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys);
	virtual void OnPaint_Impl(HWND hwnd) = 0;
	virtual void OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput);
	virtual int OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg);
	virtual void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy);
	virtual void OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status);
	virtual void OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, UINT code, int pos);

protected:

	WinAPI();
	virtual ~WinAPI() noexcept(false);

	// Static function which handle WinAPI messages to corresponding member function of the control
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/* This function is the savior...
	Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam);

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	unsigned int GetId() const noexcept;
	void SetMouseOverState(bool state) noexcept;
	void SetClickingState(bool state) noexcept;

public:

#if _DEBUG

	// Setting a static MessageMapper would cause memory leak because class destructor would not destroy it
	MessageMapper m;
#endif
	
	bool IsMouseOver() const noexcept;
	bool IsClicking() const noexcept;
	virtual bool IsEnabled() const noexcept;
	void Enable();
	void Disable();
	virtual void Update() const;
};