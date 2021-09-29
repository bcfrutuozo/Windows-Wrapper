#pragma once

#include "Control.h"
#include "ListBox.h"
#include "ComboBox.h"
#include "MenuBar.h"
#include "Button.h"
#include "TextBox.h"
#include "ProgressBar.h"
#include "ToolStrip.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"
#include "Color.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <memory>
#include <vector>
#include <optional>
#include <string>

class Window final : public Control
{
	friend class Menu;
	friend class WinAPI;

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

	// Events implementations
	void OnPaint_Impl(HWND hwnd) noexcept override;

protected:

	void Initialize() override;

public:

	Window(const std::string& name, int width, int height);
	Window(const Window&) = default;
	Window& operator=(const Window&) = default;
	virtual ~Window() noexcept(false) = default;

	void OnClosedSet(const std::function<void(Object*, OnClosedEventArgs*)>& callback) noexcept;
	void OnClosingSet(const std::function<void(Object*, OnClosingEventArgs*)>& callback) noexcept;
	void OnShownSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	// MenuStrip functions
	void ClearMenuStrip() noexcept;
	void UpdateMenuStrip() noexcept;
	//MenuBar& GetMenuStrip() noexcept;

	Button* AddButton(const std::string& name, int width, int height, int x, int y) noexcept;
	TextBox* AddTextBox(const std::string& name, int width, int x, int y) noexcept;
	ListBox* AddListBox(int width, int height, int x, int y) noexcept;
	ComboBox* AddComboBox(const std::string& name, int width, int x, int y) noexcept;
	ProgressBar* AddProgressBar(int width, int height, int x, int y) noexcept;
	ProgressBar* AddProgressBar(const std::string& text, int width, int height, int x, int y) noexcept;
	ToolStrip* AddToolStrip() noexcept;
	Label* AddLabel(const std::string& text, int x, int y) noexcept;

	void SetText(const std::string& text);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	Keyboard& GetKeyboard() const;
	Mouse& GetMouse() const;
};