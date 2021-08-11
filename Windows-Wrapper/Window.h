#pragma once

#include "WinControl.h"
#include "Color.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MenuStrip.h"
#include "Button.h"
#include "TextBox.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"

#include <memory>
#include <vector>
#include <optional>
#include <string>

class Menu;

class Window : public WinControl, public IHidable
{
	friend class Button;
	friend class Menu;
	friend class WinControl;

private:

	// Singleton manages registration/cleanup of window class
	class WindowClass
	{
	private:

		static constexpr const char* m_ClassName = "Window Class";
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

	bool m_IsCursorEnabled;
	bool m_IsMenuStripEnabled;
	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::vector<BYTE> m_RawBuffer;

	void EncloseCursor() const noexcept;
	static void FreeCursor() noexcept;
	static void HideCursor() noexcept;
	static void ShowCursor() noexcept;

	// Events implementations
	void OnCommand_Impl(HWND hwnd, int id, HWND hwndCtl, unsigned int codeNotify) noexcept override;

protected:

	void Initialize() noexcept override;

public:

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
	TextBox& AddTextBox(const std::string& name, int width, int x, int y) noexcept;

	void SetText(const std::string& text);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	Keyboard& GetKeyboard() noexcept;
	Mouse& GetMouse() noexcept;
};