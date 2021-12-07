#pragma once

#include "Component.h"
#include "NativeWindow.h"
#include "Utilities.h"
#include "Padding.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "KeyPressEventHandler.h"
#include "MouseEventHandler.h"
#include "PaintEventHandler.h"
#include "KeyEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyPressEventArgs.h"
#include "CancelEventArgs.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"
#include "PaintEventArgs.h"
#include "ControlException.h"
#include "Rectangle.h"
#include "IWindowTarget.h"
#include "ControlStyles.h"

#include <list>
#include <memory>
#include <functional>

class Window;

class Control : public Component
{
	friend class Window;
	friend class Button;
	friend class ToolStrip;
	friend class TextBox;
	friend class ListBox;

private:

	class ControlNativeWindow final : public NativeWindow, public IWindowTarget
	{
		friend class Control;

	private:

		Control* m_Control;
		IWindowTarget* m_Target;

		ControlNativeWindow(Control* control);

		constexpr const Control* GetControl() const noexcept { return m_Control; }

	protected:

		void OnHandleChange() override;
		void WndProc(Message& m) override;

	public:

		void OnHandleChange(IntPtr newHandle) override;
		void OnMessage(Message& m) override;

		constexpr IWindowTarget* GetWindowTarget() const noexcept { return m_Target; }
		constexpr void SetWindowTarget(IWindowTarget* const value) noexcept { m_Target = value; }
	};

	ControlNativeWindow* m_Window;
	CreateParams* m_CreateParams;

	int m_State;
	static constexpr int STATE_CREATED = 0x00000001;
	static constexpr int STATE_VISIBLE = 0x00000002;
	static constexpr int STATE_ENABLED = 0x00000004;
	static constexpr int STATE_TABSTOP = 0x00000008;
	static constexpr int STATE_RECREATE = 0x00000010;
	static constexpr int STATE_MODAL = 0x00000020;
	static constexpr int STATE_ALLOWDROP = 0x00000040;
	static constexpr int STATE_DROPTARGET = 0x00000080;
	static constexpr int STATE_NOZORDER = 0x00000100;
	static constexpr int STATE_LAYOUTDEFERRED = 0x00000200;
	static constexpr int STATE_USEWAITCURSOR = 0x00000400;
	static constexpr int STATE_DISPOSED = 0x00000800;
	static constexpr int STATE_DISPOSING = 0x00001000;
	static constexpr int STATE_MOUSEENTERPENDING = 0x00002000;
	static constexpr int STATE_TRACKINGMOUSEEVENT = 0x00004000;
	static constexpr int STATE_THREADMARSHALLPENDING = 0x00008000;
	static constexpr int STATE_SIZELOCKEDBYOS = 0x00010000;
	static constexpr int STATE_CAUSESVALIDATION = 0x00020000;
	static constexpr int STATE_CREATINGHANDLE = 0x00040000;
	static constexpr int STATE_TOPLEVEL = 0x00080000;
	static constexpr int STATE_ISACCESSIBLE = 0x00100000;
	static constexpr int STATE_OWNCTLBRUSH = 0x00200000;
	static constexpr int STATE_EXCEPTIONWHILEPAINTING = 0x00400000;
	static constexpr int STATE_LAYOUTISDIRTY = 0x00800000;
	static constexpr int STATE_CHECKEDHOST = 0x01000000;
	static constexpr int STATE_HOSTEDINDIALOG = 0x02000000;
	static constexpr int STATE_DOUBLECLICKFIRED = 0x04000000;
	static constexpr int STATE_MOUSEPRESSED = 0x08000000;
	static constexpr int STATE_VALIDATIONCANCELLED = 0x10000000;
	static constexpr int STATE_PARENTRECREATING = 0x20000000;
	static constexpr int STATE_MIRRORED = 0x40000000;

	ControlStyles m_ControlStyle;

	void SetHandle(IntPtr value);

protected:

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;
	bool m_IsMouseOver;
	bool m_IsClicking;
	bool m_Enabled;
	bool m_HasFontChanged;
	static IntPtr m_OpenedControl;

	// Scrolling
	int m_HorizontalScrolling;
	int m_VerticalScrolling;
	int m_VerticalScrollingUnit;
	int m_VerticalScrollPaging;

	Control* Parent;
	std::string Text;
	Padding m_Margin;
	Padding m_Padding;
	unsigned int m_MinSize;

	Size m_Size;
	Point m_Location;
	Graphics* m_Graphics;
	Font m_Font;
	Color m_ForeColor;
	Color m_BackgroundColor;

	bool m_IsVisible;

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

	// Tabulation fields
	bool m_IsTabSelected;
	bool m_IsTabStop;
	int m_TabIndex;
	static int m_IncrementalTabIndex;

	std::string Name;
	Drawing::Rectangle m_DrawableArea;

	EventHandler* OnActivate;
	EventHandler* OnClick;
	EventHandler* OnDeactivate;
	EventHandler* OnGotFocus;
	EventHandler* OnLostFocus;
	KeyEventHandler* OnKeyDown;
	KeyPressEventHandler* OnKeyPress;
	KeyEventHandler* OnKeyUp;
	MouseEventHandler* OnMouseClick;
	MouseEventHandler* OnMouseDown;
	EventHandler* OnMouseEnter;
	EventHandler* OnMouseHover;
	EventHandler* OnMouseLeave;
	MouseEventHandler* OnMouseLeftDoubleClick;
	MouseEventHandler* OnMouseMove;
	MouseEventHandler* OnMouseRightDoubleClick;
	MouseEventHandler* OnMouseUp;
	MouseEventHandler* OnMouseWheel;
	PaintEventHandler* OnPaint;
	EventHandler* OnVisibleChanged;

	Control() noexcept;
	Control(Control* parent, const std::string& text) noexcept;
	Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const std::string& text) noexcept;
	Control(const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const Control&) = default;														
	Control(Control&&) = default;															
	Control& operator=(const Control&) = default;											
	Control& operator=(Control&&) = default;	

	virtual void CreateHandle();
	virtual CreateParams* CreateParameters();

	// Default PreDraw function which is used to recalculate elements according to DataSource, number of elements, etc...
	virtual void PreDraw(Graphics* const graphics);

	// Private Draw function called to base draw the control
	// Rectangle is not passed as reference because it will be modified to draw elements while the user must receive the original one
	virtual void Draw(Graphics* const graphics, Drawing::Rectangle rectangle) = 0;

	// Perform the PostDraw to clear drawing objects
	virtual void PostDraw(Graphics* const graphics);

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
	virtual void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext);
	virtual int OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys);
	virtual void OnNotifyMessage(Message& m);
	virtual void OnPaint_Impl(HWND hwnd);
	virtual void OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput);
	virtual int OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg);
	virtual void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy);
	virtual void OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status);
	virtual void OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, UINT code, int pos);

	unsigned int GetId() const noexcept;
	void SetMouseOverState(bool state) noexcept;
	void SetClickingState(bool state) noexcept;

	IntPtr SendControlMessage(int msg, int wparam, int lparam);
	IntPtr SendControlMessage(int msg, int& wparam, int& lparam);
	IntPtr SendControlMessage(int msg, int wparam, IntPtr lparam);
	IntPtr SendControlMessage(int msg, IntPtr wparam, IntPtr lparam);
	IntPtr SendControlMessage(int msg, IntPtr wparam, int lparam);
	IntPtr SendControlMessage(int msg, int wparam, LPRECT lparam);
	IntPtr SendControlMessage(int msg, bool wparam, int lparam);
	IntPtr SendControlMessage(int msg, int wparam, const std::string& lparam);

	int GetWindowStyle();
	void SetWindowStyle(IntPtr value);

	constexpr bool GetState(int flag) { return (m_State & flag) != 0; }
	constexpr void SetState(int flag, bool value) { m_State = value ? m_State | flag : m_State & ~flag; }
	constexpr bool GetStyle(ControlStyles flag) { return (m_ControlStyle & flag) == 0; }
	constexpr void SetStyle(ControlStyles flag, bool value) { m_ControlStyle = value ? m_ControlStyle | flag : m_ControlStyle & ~flag; }

	// MOUSEENTER Event handling
	LPTRACKMOUSEEVENT m_TrackMouseEvent;
	void HookMouseEvent();
	void UnhookMouseEvent();
	
	virtual void DefWndProc(Message& m);
	virtual void WndProc(Message& m);

public:

	std::list<Control*> Controls;

	virtual ~Control() noexcept(false);																		
	void Dispose() override;

	void OnActivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnClickSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnDeactivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnGotFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnLostFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnKeyDownSet(const std::function<void(Object*, KeyEventArgs*) >& callback) noexcept;
	void OnKeyPressSet(const std::function<void(Object*, KeyPressEventArgs*) >& callback) noexcept;
	void OnKeyUpSet(const std::function<void(Object*, KeyEventArgs*) >& callback) noexcept;
	void OnMouseClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseDownSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseEnterSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseHoverSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseLeaveSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseLeftDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseMoveSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseRightDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseUpSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseWheelSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnPaintSet(const std::function<void(Object*, PaintEventArgs*)>& callback) noexcept;
	void OnVisibleChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	bool HasChildren() const noexcept;
	bool IsEnabled() const noexcept;
	Window* GetWindow() noexcept;
	Padding GetMargin() const noexcept;
	void SetMargin(Padding margin) noexcept;
	void SetMargin(int left, int top, int right, int bottom) noexcept;
	Control* GetByTabIndex(const int& index) noexcept;
	bool IsTabSelected() const noexcept;
	const std::string& GetText() const noexcept;
	void SetText(const std::string& text) noexcept;
	Control* GetPreviousControl() noexcept;
	Control* GetNextControl() noexcept;
	Control* GetById(unsigned int id) noexcept;
	Control* GetByHandle(const IntPtr p) noexcept;
	int GetTabIndex() const noexcept;
	void SetTabIndex(const int& index) noexcept;
	void SetFont(Font font) noexcept;
	void EnableTabStop() noexcept;
	void DisableTabStop() noexcept;
	bool IsTabStop() const noexcept;
	Drawing::Rectangle const GetDrawableArea() noexcept;
	void Resize(Size size);
	void Resize(int width, int height);
	bool IsHandleCreated() const noexcept;
	IntPtr GetHandle() noexcept;
	Point GetLocation() const noexcept;
	void SetLocation(Point p) noexcept;
	void SetLocation(int x, int y) noexcept;
	Size GetSize() const noexcept;
	bool IsMouseOver() const noexcept;
	bool IsClicking() const noexcept;
	void Enable();
	void Disable();
	virtual void Update();
	const Graphics* CreateGraphics() const noexcept;
	bool IsShown() const noexcept;
	virtual void Hide();
	virtual void Show();
	Font GetFont() const noexcept;
	Color GetBackgroundColor() const noexcept;
	void SetBackgroundColor(const Color& color) noexcept;
	Color GetForeColor() const noexcept;
	void SetForeColor(const Color& color) noexcept;

	// MOUSEENTER Event Handling
	void ResetMouseEventArgs();

	constexpr bool IsTopLevel() const noexcept { return Parent == nullptr; }
	constexpr Drawing::Rectangle GetClientRectangle() { return Drawing::Rectangle(0, 0, m_Size.Width, m_Size.Height); }
};