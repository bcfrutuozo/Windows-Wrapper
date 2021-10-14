#pragma once

#include "Component.h"
#include "IHandle.h"
#include "Event.h"
#include "Font.h"
#include "Color.h"
#include "OnClosingEventArgs.h"
#include "OnClosedEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyEventArgs.h"
#include "KeyPressEventArgs.h"
#include "Rectangle.h"
#include "Message.h"
#include "Point.h"
#include "Size.h"
#include "CreateParams.h"

#include <map>
#include <string>
#include <Windows.h>

class Graphics;
class Window;

class NativeWindow : public Component, public IWin32Window
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

public:

	virtual void Dispose() override;
	virtual void CreateHandle(CreateParams& cp);

	void DefWndProc(Message& m);
	Point GetLocation() const noexcept;
	void SetLocation(Point p) noexcept;
	void SetLocation(int x, int y) noexcept;
	Size GetSize() const noexcept;
	virtual void Resize(Size s);
	virtual void Resize(int width, int height);
	bool IsMouseOver() const noexcept;
	bool IsClicking() const noexcept;
	virtual bool IsEnabled() const noexcept;
	void Enable();
	void Disable();
	virtual void Update() const;
	const Graphics* CreateGraphics() const noexcept;
	bool IsShown() const noexcept;
	virtual void Hide();
	virtual void Show();
	Font GetFont() const noexcept;
	virtual void SetFont(Font font) noexcept;
	Color GetBackgroundColor() const noexcept;
	void SetBackgroundColor(const Color& color) noexcept;
	Color GetForeColor() const noexcept;
	void SetForeColor(const Color& color) noexcept;

protected:

	NativeWindow(int width, int height, int x, int y);
	virtual ~NativeWindow() noexcept(false);

	// Static function which handle WinAPI messages to corresponding member function of the control
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/* This function is the savior...
	Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam);

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI Callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	unsigned int GetId() const noexcept;
	void SetMouseOverState(bool state) noexcept;
	void SetClickingState(bool state) noexcept;
	virtual void WndProc(Message& m);
	virtual void OnHandleChange();

private:

	// Singleton manages registration/cleanup of window class
	class WindowClass : public Object
	{
		friend class NativeWindow;

	private:

		static WindowClass* m_Cache;

		WindowClass* m_Next;
		std::string m_ClassName;
		int m_ClassStyle;
		std::string m_WindowClassName;
		int m_HashCode;
		IntPtr m_DefaultWindowProc;
		WNDPROC m_WindowWNDPROC;
		bool m_IsRegistered;
		NativeWindow* m_TargetWindow;

	private:

		WindowClass(const std::string& className, int classStyle) noexcept;
		~WindowClass() = default;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		std::string GetFullClassName(const std::string& className);
		void RegisterWindowClass();
		void UnregisterWindowClass();
		void DisposeCache();

		static WindowClass* Create(const std::string& className, int classStyle);
	};

	static constexpr int primes[] = {
			11,17,23,29,37,47,59,71,89,107,131,163,197,239,293,353,431,521,631,761,919,
			1103,1327,1597,1931,2333,2801,3371,4049,4861,5839,7013,8419,10103,12143,14591,
			17519,21023,25229,30293,36353,43627,52361,62851,75431,90523, 108631, 130363,
			156437, 187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403,
			968897, 1162687, 1395263, 1674319, 2009191, 2411033, 2893249, 3471899, 4166287,
			4999559, 5999471, 7199369
	};

#if _DEBUG
	std::string subclassStatus = "None";
#endif

	static bool m_AnyHandleCreated;
	static bool m_AnyHandleCreatedInApp;
	static int m_GlobalID;
	static std::map<void*, int> m_WindowMap;
	static std::map<short, void*> m_HashForIdHandle;
	static std::map<void*, short> m_HashForHandleId;
	bool m_OwnHandle;
	NativeWindow* m_PreviousWindow; // doubly linked list of subclasses.
	NativeWindow* m_NextWindow;
	static IntPtr m_WindowProc;
	static IntPtr m_UserDefinedWindowProc;
	IntPtr m_DefaultWindowProc;
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

	void ReleaseHandle(bool handleValid);
	void UnSubClass();
	void CheckReleased() const;
	static unsigned int InitHash(IntPtr handle, int hashsize, unsigned int& seed, unsigned int& incr);
	static void AddWindowToTable(IntPtr handle, NativeWindow* window);
	static void RemoveWindowFromTable(IntPtr handle, NativeWindow* window);
	static void AddWindowToIDTable(IntPtr handle);
	static void RemoveWindowFromIDTable(IntPtr handle);

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
	virtual void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) = 0;
	virtual int OnNotify_Impl(HWND hwnd, int xPos, int yPos, int zDelta, unsigned int fwKeys);
	virtual void OnPaint_Impl(HWND hwnd);
	virtual void OnRawInput_Impl(HWND hWnd, unsigned int inputCode, HRAWINPUT hRawInput);
	virtual int OnSetCursor_Impl(HWND hwnd, HWND hwndCursor, unsigned int codeHitTest, unsigned int msg);
	virtual void OnSize_Impl(HWND hwnd, unsigned int state, int cx, int cy);
	virtual void OnShowWindow_Impl(HWND hwnd, bool fShow, unsigned int status);
	virtual void OnVerticalScrolling_Impl(HWND hwnd, HWND hwndCtl, UINT code, int pos);
};