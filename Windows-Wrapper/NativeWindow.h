#pragma once

#include "Object.h"
#include "IWin32Window.h"
#include "Rectangle.h"
#include "Message.h"
#include "CreateParams.h"

#include <map>
#include <string>

#define WM_MOUSEENTER NativeWindow::GetWM_MOUSEENTER()
#define WM_UIUNSUBCLASS NativeWindow::GetWM_UIUNSUBCLASS()

class NativeWindow : public Object, public IWin32Window
{
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
		WNDPROC m_WindowProc;
		bool m_IsRegistered;
		NativeWindow* m_TargetWindow;

		WindowClass(const std::string& className, int classStyle) noexcept;
		~WindowClass() = default;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		std::string GetFullClassName(const std::string& className);
		void RegisterWindowClass();
		void UnregisterWindowClass();
		void DisposeCache();

		// Static function which handle WinAPI messages to corresponding member function of the control
		static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/* This function is the savior...
		Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
		static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam);

		static WindowClass* Create(const std::string& className, int classStyle);
	};

#if _DEBUG
	std::string subclassStatus = "None";
#endif

	// Field to register WM_MOUSEENTER message
	static int m_WMMOUSEENTER;

	// Field to register WM_UIUNSUBCLASS message
	static int m_WMUNSUBCLASS;

	static std::map<int, std::string_view> CustomMessages;

	static bool m_AnyHandleCreated;
	static bool m_AnyHandleCreatedInApp;
	static int m_HandleCount;
	static int m_GlobalID;
	static std::map<void*, NativeWindow*> m_WindowMap;
	static std::map<short, void*> m_HashForIdHandle;
	static std::map<void*, short> m_HashForHandleId;
	bool m_OwnHandle;
	NativeWindow* m_PreviousWindow;
	NativeWindow* m_NextWindow;
	IntPtr m_WindowProcPtr;
	WNDPROC m_WindowProc;
	static IntPtr m_UserDefinedWindowProc;
	IntPtr m_DefaultWindowProc;

	void AssignHandle(IntPtr handle, bool assignUniqueID);
	void ReleaseHandle(bool handleValid);
	void UnSubClass();
	void CheckReleased() const;
	static void AddWindowToTable(IntPtr handle, NativeWindow* window);
	static void RemoveWindowFromTable(IntPtr handle, NativeWindow* window);
	static void AddWindowToIDTable(IntPtr handle);
	static void RemoveWindowFromIDTable(IntPtr handle);
	static NativeWindow* GetWindowFromTable(IntPtr handle);

protected:

	NativeWindow();
	virtual ~NativeWindow() noexcept(false);

	// Member function responsible to handle the messages of each different type of control 
	LRESULT WINAPI Callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void WndProc(Message& m);
	virtual void OnHandleChange();

public:

	static int GetWM_MOUSEENTER() noexcept;
	static int GetWM_UIUNSUBCLASS() noexcept;

	virtual void CreateHandle(CreateParams* cp);
	void DefWndProc(Message& m);
	void AssignHandle(IntPtr handle);
	NativeWindow* GetPreviousWindow();
	static NativeWindow* FromHandle(IntPtr handle);
};