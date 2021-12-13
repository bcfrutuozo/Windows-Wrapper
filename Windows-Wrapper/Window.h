#pragma once

#include "ContainerControl.h"
#include "ListBox.h"
#include "ComboBox.h"
#include "MenuBar.h"
#include "Button.h"
#include "TextBox.h"
#include "ProgressBar.h"
#include "ToolStrip.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "FormBorderStyle.h"
#include "FormWindowState.h"
#include "FormStartPosition.h"
#include "MDIClient.h"

class Window final : public ContainerControl
{
	friend class Menu;
	friend class NativeWindow;

private:

	bool m_FormStateShowWindowOnCreate;
	int m_FormStateSetClientSize;
	int m_FormStateMdiChildMax;
	bool m_HasControlBox;
	bool m_HasMaximizeBox;
	bool m_HasMinimizeBox;
	bool m_HasHelpButton;
	bool m_IsLayered;
	bool m_HasTaskBar;
	bool m_IsCursorEnabled;
	bool m_IsMenuStripEnabled;
	bool m_IsIconShown;
	bool m_IsMDI;
	bool m_UseMDIChildProc;
	Drawing::Rectangle m_RestoreBounds;
	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::vector<BYTE> m_RawBuffer;
	FormBorderStyle m_FormBorderStyle;
	FormWindowState m_FormWindowState;
	FormStartPosition m_FormStartPosition;
	Window* m_ActiveMDIChild;
	MDIClient* m_CtlClient;
	Window* m_MDIParent;
	NativeWindow* m_OwnerWindow;
	Drawing::Rectangle m_RestoredWindowBounds;
	BoundsSpecified m_RestoredWindowBoundsSpecified;

	void FillInCreateParamsBorderStyles(CreateParams* cp);
	void FillInCreateParamsWindowState(CreateParams* cp);
	void FillInCreateParamsBorderIcons(CreateParams* cp);
	void FillInCreateParamsStartPosition(CreateParams* cp);
	Size ComputeWindowSize(Size clientSize);
	Size ComputeWindowSize(Size clientSize, int style, int exStyle);
	
	void EncloseCursor() noexcept;
	static void FreeCursor() noexcept;
	static void HideCursor() noexcept;
	static void ShowCursor() noexcept;
	bool HasMenu() noexcept;
	void UpdateWindowState();

	//void Draw(Graphics* const graphics, Drawing::Rectangle rectangle) override;

	void WmCreate(Message& m);
	void WmEraseBackground(Message& m);
	void WmNcButtonDown(Message& m);
	void WmWinPosChanged(Message& m);

protected:

	void DefWndProc(Message& m) override;
	void WndProc(Message& m) override;
	
	void SetBoundsCore(int x, int y, int width, int height, BoundsSpecified specified) override;
	//void Initialize() override;

public:

	Window();
	Window(const Window&) = default;
	Window& operator=(const Window&) = default;
	virtual ~Window() noexcept(false) = default;

	CreateParams* CreateParameters() override;
	void OnClosedSet(const std::function<void(Object*, OnClosedEventArgs*)>& callback) noexcept;
	void OnClosingSet(const std::function<void(Object*, OnClosingEventArgs*)>& callback) noexcept;
	void OnShownSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	// MenuStrip functions
	void ClearMenuStrip() noexcept;
	void UpdateMenuStrip() noexcept;
	//MenuBar& GetMenuStrip() noexcept;

	//Button* AddButton(const std::string& name, int width, int height, int x, int y) noexcept;
	//TextBox* AddTextBox(const std::string& name, int width, int x, int y) noexcept;
	//ListBox* AddListBox(int width, int height, int x, int y) noexcept;
	//ComboBox* AddComboBox(const std::string& name, int width, int x, int y) noexcept;
	//ProgressBar* AddProgressBar(int width, int height, int x, int y) noexcept;
	//ProgressBar* AddProgressBar(const std::string& text, int width, int height, int x, int y) noexcept;
	//ToolStrip* AddToolStrip() noexcept;
	//Label* AddLabel(const std::string& text, int x, int y) noexcept;

	void SetText(const std::string& text);
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool IsCursorEnabled() const noexcept;
	Keyboard& GetKeyboard() const;
	Mouse& GetMouse() const;
	FormWindowState WindowState();
	void WindowState(FormWindowState value);
	FormBorderStyle GetFormBorderStyle();
	void SetFormBorderStyle(FormBorderStyle value);

	constexpr bool HasControlBox() const noexcept { return m_HasControlBox; }
	constexpr void EnableControlBox() noexcept { m_HasControlBox = 1; }
	constexpr void DisableControlBox() noexcept { m_HasControlBox = 0; }
 
	constexpr bool IsIconShown() const noexcept { return m_IsIconShown; }
	constexpr void EnableIcon() noexcept { m_IsIconShown = 1; }
	constexpr void DisableIcon() noexcept { m_IsIconShown = 0; }

	constexpr bool IsMDIChild() const noexcept { return m_MDIParent != nullptr; }
	constexpr Window* GetActiveMDIChild() const noexcept { return m_ActiveMDIChild; }
	constexpr MDIClient* GetMDIClient() const noexcept { return m_CtlClient; }
	constexpr Window* GetMDIParent() const noexcept { return m_MDIParent; }
};