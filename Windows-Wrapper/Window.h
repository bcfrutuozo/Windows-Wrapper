#pragma once

#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <Windows.h>
#include <memory>
#include <vector>
#include <optional>
#include <string>

// Error exception helper macro
#define WND_EXCEPT( hr ) Window::HRException( __LINE__,__FILE__,(hr) )
#define WND_LAST_EXCEPT() Window::HRException( __LINE__,__FILE__,GetLastError() )

class Window
{
private:

	// Singleton manages registration/cleanup of window class
	class WndClass
	{
	private:

		WndClass() noexcept;
		~WndClass() noexcept;
		WndClass(const WndClass&) = delete;
		WndClass& operator=(const WndClass&) = delete;

		static constexpr const char* m_ClassName = "Window Class";
		static WndClass m_WndClass;
		HINSTANCE m_Instance;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	void EncloseCursor() const noexcept;
	static void FreeCursor() noexcept;
	static void HideCursor() noexcept;
	static void ShowCursor() noexcept;
	static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMessageForwarder(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	bool m_IsCursorEnabled;
	int m_Width;
	int m_Height;
	HWND m_Handle;
	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::vector<BYTE> rawBuffer;

public:

	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const char* title);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	Keyboard& GetKeyboard() noexcept;
	Mouse& GetMouse() noexcept;
	static const std::optional<int> ProcessMessages();

	// Window Exception
	class WindowException : public Exception
	{
	public:
		static const std::string& TranslateErrorCode(HRESULT hr) noexcept;
	};

	// HRException
	class HRException : public Exception
	{
	public:

		HRException(int line, const char* file, HRESULT hr) noexcept;

		const char* what() const noexcept override;
		const char* GetType() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		const std::string& GetErrorDescription() const noexcept;

	private:

		HRESULT hr;
	};
};

