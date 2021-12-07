#pragma once

#include "NativeWindow.h"
#include "Application.h"

#include <sstream>

// Non-default WinAPI messages registration
int NativeWindow::m_WMMOUSEENTER = -1;
int NativeWindow::GetWM_MOUSEENTER() noexcept
{
	if(m_WMMOUSEENTER == -1)
	{
		m_WMMOUSEENTER = RegisterWindowMessage("Win32WrapperMouseEnter");
		CustomMessages[m_WMMOUSEENTER] = "WM_MOUSEENTER";
	}

	return m_WMMOUSEENTER;
}

int NativeWindow::m_WMUNSUBCLASS = -1;
int NativeWindow::GetWM_UIUNSUBCLASS() noexcept
{
	if(m_WMUNSUBCLASS == -1)
	{
		m_WMUNSUBCLASS = RegisterWindowMessage("Win32WrapperUnSubclass");
		CustomMessages[m_WMUNSUBCLASS] = "WM_UIUNSUBCLASS";
	}
	return m_WMUNSUBCLASS;
}

int NativeWindow::m_GlobalID = 1;
std::map<int, std::string_view> NativeWindow::CustomMessages = std::map<int, std::string_view>();
std::map<void*, NativeWindow*> NativeWindow::m_WindowMap = std::map<void*, NativeWindow*>();
std::map<short, void*> NativeWindow::m_HashForIdHandle = std::map<short, void*>();
std::map<void*, short> NativeWindow::m_HashForHandleId = std::map<void*, short>();
IntPtr NativeWindow::m_UserDefinedWindowProc = nullptr;
NativeWindow::WindowClass* NativeWindow::WindowClass::m_Cache = nullptr;
bool NativeWindow::m_AnyHandleCreated = false;
bool NativeWindow::m_AnyHandleCreatedInApp = false;

std::string NativeWindow::WindowClass::GetFullClassName(const std::string& className)
{
	std::ostringstream oss;

	oss << className << ".app.";

	return oss.str();
}

void NativeWindow::WindowClass::RegisterWindowClass()
{
	WNDCLASSEX wndClass = { 0 };

	if(m_UserDefinedWindowProc == IntPtr::Zero())
	{
	#if UNICODE
		const std::string defaultProc = "DefWindowProcW";
	#else
		const std::string defaultProc = "DefWindowProcA";
	#endif

		m_UserDefinedWindowProc = GetProcAddress(GetModuleHandle("user32.dll"), TEXT(defaultProc.c_str()));
		if(m_UserDefinedWindowProc == IntPtr::Zero()) throw Win32Exception();
	}

	std::ostringstream localClassName;

	if(m_ClassName.empty())
	{
		// If we don't use a hollow brush here, Windows will "pre paint" us with COLOR_WINDOW which
		// creates a little bit if flicker.  This happens even though we are overriding wm_erasebackgnd.
		// Make this hollow to avoid all flicker.
		wndClass.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH); //(IntPtr)(NativeMethods.COLOR_WINDOW + 1);
		wndClass.style = m_ClassStyle;

		m_DefaultWindowProc = m_UserDefinedWindowProc;
		localClassName << "Window." << "0x" << std::hex << m_ClassStyle;
		m_HashCode = 0;
	}

	else
	{
		WNDCLASSA wcls;
		bool ok = GetClassInfo(nullptr, m_ClassName.c_str(), &wcls);
		int error = GetLastError();

		if(!ok) throw Win32Exception(error, "InvalidWndClsName");

		wndClass.style = wcls.style;
		wndClass.cbClsExtra = wcls.cbClsExtra;
		wndClass.cbWndExtra = wcls.cbWndExtra;
		wndClass.hIcon = wcls.hIcon;
		wndClass.hCursor = wcls.hCursor;
		wndClass.hbrBackground = wcls.hbrBackground;
		wndClass.lpszMenuName = wcls.lpszMenuName;
		localClassName << m_ClassName;
		m_DefaultWindowProc = wcls.lpfnWndProc;
		m_HashCode = GetHashCode();
	}

	// Our static data is different for different app domains, so we include the app domain in with
	// our window class name.  This way our static table always matches what Win32 thinks.
	m_WindowClassName = GetFullClassName(localClassName.str());
	m_WindowProc = HandleMessageSetup;
	wndClass.lpfnWndProc = m_WindowProc;
	wndClass.hInstance = GetModuleHandle(nullptr);
	wndClass.lpszClassName = m_WindowClassName.c_str();
	wndClass.cbSize = sizeof(WNDCLASSEX);

	short atom = RegisterClassEx(&wndClass);

	if(atom == 0)
	{
		int err = GetLastError();

		if(err == ERROR_CLASS_ALREADY_EXISTS)
		{
			// Check to see if the window class window
			// proc points to DefWndProc.  If it does, then
			// this is a class from a rudely-terminated app domain
			// and we can safely reuse it.  If not, we've got
			// to throw.
			WNDCLASSA wcls;
			bool ok = GetClassInfo(GetModuleHandle(nullptr), m_WindowClassName.c_str(), &wcls);
			if(ok && wcls.lpfnWndProc == m_UserDefinedWindowProc)
			{
				// We can just reuse this class because we have marked it
				// as being a nop in another domain.  All we need to do is call SetClassLong.
				// Only one problem:  SetClassLong takes an HWND, which we don't have.  That leaves
				// us with some tricky business. First, try this the easy way and see
				// if we can simply unregister and re-register the class.  This might
				// work because the other domain shutdown would have posted WM_CLOSE to all
				// the windows of the class.
				if(UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr)))
				{
					// If this fails, we will always raise the first err above.  No sense exposing our twiddling.
					atom = RegisterClassEx(&wndClass);
				}
				else
				{
					// This is a little harder.  We cannot reuse the class because it is
					// already in use.  We must create a new class.  We bump our domain qualifier
					// here to account for this, so we only do this expensive search once for the
					// domain.  
					do
					{
						m_WindowClassName = GetFullClassName(localClassName.str());
						wndClass.lpszClassName = m_WindowClassName.c_str();
						atom = RegisterClassEx(&wndClass);
					} while(atom == 0 && GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
				}
			}
		}

		if(atom == 0)
		{
			m_WindowProc = nullptr;
			throw Win32Exception(err);
		}
	}

	m_IsRegistered = true;
}

void NativeWindow::WindowClass::UnregisterWindowClass()
{
	if(m_IsRegistered && UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(nullptr)))
	{
		m_WindowProc = nullptr;
		m_IsRegistered = false;
	}
}

void NativeWindow::WindowClass::DisposeCache()
{
	WindowClass* wc = m_Cache;

	while(wc != nullptr)
	{
		WindowClass* oldWc = wc;
		wc->UnregisterWindowClass();
		wc = wc->m_Next;
		SafeDelete(oldWc);
	}
}

NativeWindow::WindowClass* NativeWindow::WindowClass::Create(const std::string& className, int classStyle)
{
	WindowClass* wc = m_Cache;

	if(className.empty())
	{
		while(wc != nullptr && (!wc->m_ClassName.empty() || wc->m_ClassStyle != classStyle))
			wc = wc->m_Next;
	}
	else
	{
		while(wc != nullptr && className != wc->m_ClassName)
			wc = wc->m_Next;
	}

	if(wc == nullptr)
	{
		wc = new WindowClass(className, classStyle);
		wc->m_Next = m_Cache;
		m_Cache = wc;
	}
	else
	{
		if(!wc->m_IsRegistered) wc->RegisterWindowClass();
	}

	return wc;
}

// Window class declarations
NativeWindow::WindowClass::WindowClass(const std::string& className, int classStyle) noexcept
	:
	m_ClassName(className),
	m_ClassStyle(classStyle)
{
	RegisterWindowClass();
}

void NativeWindow::ReleaseHandle(bool handleValid)
{
	if(Handle != IntPtr::Zero())
	{
		if(handleValid) UnSubClass();

		RemoveWindowFromTable(Handle, this);
		Handle = IntPtr::Zero();
		OnHandleChange();
	}
}

void NativeWindow::UnSubClass()
{
	bool finalizing = Handle != nullptr;

	// Don't touch if the current window proc is not ours.
	IntPtr currentWinPrc = GetWindowLong(Handle, GWLP_WNDPROC);
	if(m_WindowProcPtr == currentWinPrc)
	{
		if(m_PreviousWindow == nullptr)
		{
		#if _DEBUG
			subclassStatus = "Unsubclassing back to native defWindowProc";
		#endif
			// If the defWindowProc points to a native window proc, previousWindow will
			// be null.  In this case, it is completely safe to assign defWindowProc
			// to the current wndproc.
			SetWindowLong(Handle, GWLP_WNDPROC, m_DefaultWindowProc.ToInt64());
		}
		else
		{
			if(finalizing)
			{
			#if DEBUG
				subclassStatus = "Setting back to userDefWindowProc -- next chain is managed";
			#endif
				// Here, we are finalizing and defWindowProc is pointing to a managed object.  We must assume
				// that the object defWindowProc is pointing to is also finalizing.  Why?  Because we're
				// holding a ref to it, and it is holding a ref to us.  The only way this cycle will
				// finalize is if no one else is hanging onto it.  So, we re-assign the window proc to
				// userDefWindowProc.
				SetWindowLong(Handle, GWLP_WNDPROC, m_UserDefinedWindowProc.ToInt64());
			}
			else
			{
			#if DEBUG
				subclassStatus = "Setting back to next managed subclass object";
			#endif
				// Here we are not finalizing so we use the windowProc for our previous window.  This may
				// DIFFER from the value we are currently storing in defWindowProc because someone may
				// have re-subclassed.
				SetWindowLong(Handle, GWLP_WNDPROC, (LONG)m_PreviousWindow->m_WindowProc);
			}
		}
	}
	else
	{
		// Cutting the subclass chain anyway, even if we're not the last one in the chain
		// If the whole chain is all managed NativeWindow it doesnt matter, 
		// if the chain is not, then someone has been dirty and didn't clean up properly, too bad for them...

		// We will cut off the chain if we cannot unsubclass.
		// If we find previouswindow pointing to us, then we can let RemoveWindowFromTable reassign the
		// m_DefaultWindowProc pointers properly when this guy gets removed (thereby unsubclassing ourselves)
		if(m_NextWindow == nullptr || m_NextWindow->m_DefaultWindowProc != m_WindowProcPtr)
		{
			// We didn't find it... let's unhook anyway and cut the chain... this prevents crashes
			SetWindowLong(Handle, GWLP_WNDPROC, m_UserDefinedWindowProc.ToInt64());
		}
	#if _DEBUG
		subclassStatus = "FORCE unsubclass -- we do not own the subclass";
	#endif
	}
}

void NativeWindow::CheckReleased() const
{
	if(Handle != IntPtr::Zero()) throw InvalidOperationException("HandleAlreadyExists");
}

NativeWindow::NativeWindow()
	:
	m_PreviousWindow(nullptr),
	m_NextWindow(nullptr),
	m_OwnHandle(false),
	m_WindowProc(0L)
{

}

NativeWindow::~NativeWindow() noexcept(false)
{

}

void NativeWindow::AssignHandle(IntPtr handle, bool assignUniqueID)
{
	CheckReleased();
	assert(handle != IntPtr::Zero(), "handle is 0");

	// THIS IS WHERE THE HANDLE IS SET
	Handle = handle;

	//if(m_UserDefinedWindowProc == IntPtr::Zero())
	//{
	//#ifdef UNICODE
	//	const std::string defaultProc = "DefWindowProcW";
	//#else
	//	const std::string defproc = "DefWindowProcA";
	//#endif
	//
	//	m_UserDefinedWindowProc = GetProcAddress(GetModuleHandle("user32.dll"), TEXT(defproc.c_str()));
	//	if(m_UserDefinedWindowProc == IntPtr::Zero())
	//	{
	//		throw Win32Exception();
	//	}
	//}

	//m_DefaultWindowProc = (void*)GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWLP_WNDPROC);
	//assert(m_DefaultWindowProc != IntPtr::Zero(), "defWindowProc is 0");
	//
	//m_WindowProc = &NativeWindow::WindowClass::HandleMessageForwarder;

	AddWindowToTable(handle, this);

	//SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWLP_WNDPROC, (LONG)m_WindowProc);
	//m_WindowProcPtr = (void*)GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWLP_WNDPROC);
	//assert(m_DefaultWindowProc != m_WindowProcPtr, "Uh oh! Subclassed ourselves!!!");
	if(assignUniqueID &&
	   (((int)((long)GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_STYLE))) & WS_CHILD) != 0 &&
	   (int)((long)GetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_ID)) == 0)
	{
		SetWindowLong(static_cast<HWND>(Handle.ToPointer()), GWL_ID, Handle.ToInt64());
	}

	OnHandleChange();
}


// Static function which handle WinAPI messages to corresponding member function of the control
LRESULT WINAPI NativeWindow::WindowClass::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if(msg == WM_NCCREATE)
	{
		// Extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		NativeWindow* const pWnd = static_cast<NativeWindow*>(pCreate->lpCreateParams);

		// Set WinAPI-managed user data do store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Set message function to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NativeWindow::WindowClass::HandleMessageForwarder));

		pWnd->AssignHandle(hWnd);

		// Forward message to window class member function
		return pWnd->Callback(hWnd, msg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	//return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* This function is the savior...
Not only it handles the message to the member function but it can also forward a message from one WinControl to another */
LRESULT WINAPI NativeWindow::WindowClass::HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM lParam, LPARAM wParam)
{
	// Retrieve pointer to window class
	NativeWindow* const pWnd = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward message to window class member function
	return pWnd->Callback(hWnd, msg, lParam, wParam);
}

// Member function responsible to handle the messages of each different type of control 
LRESULT WINAPI NativeWindow::Callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Message m = Message::Create(hWnd, msg, wParam, lParam);

	try
	{
		if(Handle != nullptr)
		{
			WndProc(m);
		}
		else
		{
			DefWndProc(m);
		}
	}
	catch(const Exception& e)
	{
		//OnThreadException(e);
	}

	if(msg == WM_NCDESTROY)
	{
		ReleaseHandle(false);
	}
	
	if(msg == WM_UIUNSUBCLASS)
	{
		ReleaseHandle(true);
	}

	return m.Result;
}

void NativeWindow::WndProc(Message& m)
{
	DefWndProc(m);
}

void NativeWindow::OnHandleChange()
{}

void NativeWindow::CreateHandle(CreateParams* cp)
{
	CheckReleased();
	WindowClass* windowClass = WindowClass::Create(cp->ClassName, cp->ClassStyle);

	// Why are we checking the handle again after calling CheckReleased()?  It turns                     
	// out the CLR will sometimes pump messages while we're waiting on the lock.  If
	// a message comes through (say a WM_ACTIVATE for the parent) which causes the 
	// handle to be created, we can try to create the handle twice for the same 
	// NativeWindow object. See bug for more details.
	if(Handle != IntPtr::Zero()) return;

	windowClass->m_TargetWindow = this;
	IntPtr createResult = IntPtr::Zero();
	int lastWin32Error = 0;

	if(cp->Caption.length() > SHRT_MAX)
	{
		cp->Caption = cp->Caption.substr(0, SHRT_MAX);
	}

	createResult = CreateWindowEx(cp->ExStyle,
								  windowClass->m_WindowClassName.c_str(),
								  cp->Caption.c_str(),
								  cp->Style,
								  cp->X,
								  cp->Y,
								  cp->Width,
								  cp->Height,
								  static_cast<HWND>(cp->Parent.ToPointer()),
								  nullptr,
								  nullptr,
								  cp->Param);

	lastWin32Error = GetLastError();

	//windowClass->m_TargetWindow = nullptr;

	//if(createResult == IntPtr::Zero()) throw Win32Exception(lastWin32Error, "ErrorCreatingHandle");

	m_OwnHandle = true;
}

void NativeWindow::DefWndProc(Message& m)
{
	if(m_PreviousWindow == nullptr)
	{
		if(m_DefaultWindowProc == IntPtr::Zero())
		{

		#if _DEBUG

			/*
			Message 0xC0E8 is in the range of application-defined window messages that are registered at
			runtime with RegisterWindowMessage().

			Range					Meaning
			0 through WM_USER –1	Messages reserved for use by the system.
			WM_USER through 0x7FFF	Integer messages for use by private window classes.
			WM_APP through 0xBFFF	Messages available for use by applications.
			0xC000 through 0xFFFF	String messages for use by applications.
			Greater than 0xFFFF		Reserved by the system.
			*/

			std::ostringstream oss;
			
			if(m.Msg >= 0xC000 && m.Msg <= 0xFFFF)
			{
				oss << "Can't find a default window procedure for message " 
					<< "Msg=0x" << std::hex << m.Msg << "(" << CustomMessages[m.Msg] << ")"
					<< "hWnd=0x" << std::hex << m.hWnd.ToInt64()
					<< "wParam=0x" << std::hex << m.wParam.ToInt64()
					<< "lParam=0x" << std::hex << m.lParam.ToInt64()
					<< "Result=0x" << std::hex << m.Result.ToInt64()
					<< " on class " << GetType().ToString() << " subclass status: " << subclassStatus << std::endl;
			}
			else
			{
				oss << "Can't find a default window procedure for message " << m.ToString() << " on class " << GetType().ToString() << " subclass status: " << subclassStatus << std::endl;
			}

			printf_s(oss.str().c_str());
		#endif

			// At this point, there isn't much we can do.  There's a
			// small chance the following line will allow the rest of
			// the program to run, but don't get your hopes up.
			m.Result = (void*)DefWindowProc(m.hWnd, m.Msg, m.wParam, m.lParam);
			return;
		}
		m.Result = (void*)CallWindowProc((WNDPROC)m_DefaultWindowProc.ToPointer(), m.hWnd, m.Msg, m.wParam, m.lParam);
	}
	else
	{
		assert(m_PreviousWindow != this, "Looping in our linked list");
		m.Result = (void*)m_PreviousWindow->Callback(m.hWnd, m.Msg, m.wParam, m.lParam);
	}
}

void NativeWindow::AddWindowToTable(IntPtr handle, NativeWindow* window)
{
	assert(handle != IntPtr::Zero(), "Should never insert a zero handle into the hash");

	// set a flag that this thread is tracking an HWND
	//
	m_AnyHandleCreated = true;
	// ...same for the application
	m_AnyHandleCreatedInApp = true;

	m_WindowMap[handle.ToPointer()] = window;
}

void NativeWindow::RemoveWindowFromTable(IntPtr handle, NativeWindow* window)
{
	assert(handle != IntPtr::Zero(), "Incorrect handle");

	NativeWindow* prevWindow = window->m_PreviousWindow;

	if(window->m_PreviousWindow != nullptr)
	{
		window->m_PreviousWindow->m_NextWindow = window->m_NextWindow;
	}

	if(window->m_NextWindow != nullptr)
	{
		window->m_NextWindow->m_DefaultWindowProc = window->m_DefaultWindowProc;
		window->m_NextWindow->m_PreviousWindow = window->m_PreviousWindow;
	}

	m_WindowMap.erase(handle.ToPointer());
	SafeDelete(window);
}

void NativeWindow::AddWindowToIDTable(IntPtr handle)
{
	m_HashForIdHandle[m_GlobalID] = handle.ToPointer();
	m_HashForHandleId[handle.ToPointer()] = m_GlobalID;
	SetWindowLong(static_cast<HWND>(handle.ToPointer()), GWLP_ID, m_GlobalID);
	++m_GlobalID;
}

void NativeWindow::RemoveWindowFromIDTable(IntPtr handle)
{
	short id = m_HashForHandleId[handle.ToPointer()];
	m_HashForHandleId.erase(handle.ToPointer());
	m_HashForIdHandle.erase(id);
}

void NativeWindow::AssignHandle(IntPtr handle)
{
	AssignHandle(handle, true);
}