#pragma once

#include "Control.h"
#include "Color.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MenuStrip.h"
#include "Button.h"
#include "TextBox.h"
#include "IWinControl.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"

#include <memory>
#include <vector>
#include <optional>
#include <string>

class Menu;

class Window : public IWinControl<Window>, public IHidable
{
	friend class Button;
	friend class Menu;
	friend class IWinControl;

private:

	bool m_IsCursorEnabled;
	bool m_IsMenuStripEnabled;
	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::vector<BYTE> m_RawBuffer;

	void EncloseCursor() const noexcept;
	static void FreeCursor() noexcept;
	static void HideCursor() noexcept;
	static void ShowCursor() noexcept;

	// Member function for message handling 
	LRESULT WINAPI HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	// Events implementations
	void OnActivate_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnSetCursor_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnCommand_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnClose_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnCreate_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnEraseBackground_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnDestroy_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnClosed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnFocusEnter_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnFocusLeave_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnKeyDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnKeyPressed_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnKeyUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseMove_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseLeftDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseLeftUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseRightDown_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseRightUp_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseLeftDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseRightDoubleClick_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnMouseWheel_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	int OnNotify_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnRawInput_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void OnShowWindow_Impl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

protected:

	void Initialize() noexcept override;

public:

	// Singleton manages registration/cleanup of window class
	class WndClass
	{
	private:

		static constexpr const char* m_ClassName = "Window Class";
		static WndClass m_WndClass;
		HINSTANCE m_Instance;

		WndClass() noexcept;
		~WndClass() noexcept;
		WndClass(const WndClass&) = delete;
		WndClass& operator=(const WndClass&) = delete;

	public:

		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	};

	Window(const std::string& name, int width, int height);
	virtual ~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void Hide() override;
	void Show() override;
	void SetBackgroundColor(const Color& color) noexcept override;

	void OnClosedSet(const std::function<void(Control* const c, OnClosedEventArgs* const e)>& callback) noexcept;
	void OnClosingSet(const std::function<void(Control* const c, OnClosingEventArgs* const e)>& callback) noexcept;
	void OnShownSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;

	// MenuStrip functions
	void ClearMenuStrip() noexcept;
	void UpdateMenuStrip() noexcept;
	MenuStrip& GetMenuStrip() noexcept;

	// Buttons
	Button& AddButton(const std::string& name, int width, int height, int x, int y) noexcept;
	TextBox& AddTextBox(const std::string& name, int width, int height, int x, int y) noexcept;

	void SetText(const std::string& text);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	Keyboard& GetKeyboard() noexcept;
	Mouse& GetMouse() noexcept;
};