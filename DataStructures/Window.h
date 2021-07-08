#pragma once

#include <Windows.h>
#include <optional>

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

private:

	void EncloseCursor() const noexcept;
	static void FreeCursor() noexcept;
	static void HideCursor() noexcept;
	static void ShowCursor() noexcept;
	static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	bool m_IsCursorEnabled;
	int m_Width;
	int m_Height;
	HWND m_Handle;

public:

	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const char* title);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	bool HasNewDevice() noexcept;
	static std::optional<int> ProcessMessages();

};

