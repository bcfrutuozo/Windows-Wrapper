#pragma once

#include "CommonObject.h"

// Singleton manages registration/cleanup of window class
class WindowClass : public Object
{
private:

	static constexpr const char* m_ClassName = "Win32 API Wrapper Class";
	static WindowClass m_WindowClass;
	HINSTANCE m_Instance;

	WindowClass() noexcept;
	~WindowClass() noexcept;
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;

public:

	static const char* GetName() noexcept;
	static HINSTANCE GetInstance() noexcept;
};