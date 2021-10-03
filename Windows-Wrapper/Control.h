#pragma once

#include "WinAPI.h"
#include "Utilities.h"
#include "Padding.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "KeyPressEventHandler.h"
#include "MouseEventHandler.h"
#include "PaintEventHandler.h"
#include "KeyEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyPressEventArgs.h"
#include "CancelEventArgs.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"
#include "PaintEventArgs.h"
#include "ControlException.h"

#include <list>
#include <memory>
#include <functional>

class Control : public WinAPI
{
	friend class Window;
	friend class Button;
	friend class ToolStrip;
	friend class TextBox;
	friend class ListBox;

private:

	// Tabulation fields
	bool m_IsTabSelected;
	bool m_IsTabStop;
	int m_TabIndex;
	static int m_IncrementalTabIndex;

	std::string Name;

	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) override;
	void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) override;

	EventHandler* OnActivate;
	EventHandler* OnClick;
	EventHandler* OnDeactivate;
	EventHandler* OnGotFocus;
	EventHandler* OnLostFocus;
	KeyEventHandler* OnKeyDown;
	KeyPressEventHandler* OnKeyPress;
	KeyEventHandler* OnKeyUp;
	MouseEventHandler* OnMouseClick;
	MouseEventHandler* OnMouseDown;
	EventHandler* OnMouseEnter;
	EventHandler* OnMouseHover;
	EventHandler* OnMouseLeave;
	MouseEventHandler* OnMouseLeftDoubleClick;
	MouseEventHandler* OnMouseMove;
	MouseEventHandler* OnMouseRightDoubleClick;
	MouseEventHandler* OnMouseUp;
	MouseEventHandler* OnMouseWheel;
	PaintEventHandler* OnPaint;
	EventHandler* OnVisibleChanged;

protected:

	Control* Parent;
	std::string Text;
	Padding m_Margin;
	Padding m_Padding;
	unsigned int m_MinSize;

	// Forces the implementation and call on individual childs because each WinApi control
	// has it own creation method.
	// ALL DERIVED CLASSES MUST CALL THIS METHOD ON ITS CONSTRUCTOR
	virtual void Initialize() = 0;

	template<typename T, typename... Args>
	T* Create(Args... a)
	{
		T* item = new T(a...);
		Controls.push_back(item);
		return item;
	}

	Control() noexcept;
	Control(Control* parent, const std::string& text) noexcept;
	Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const std::string& text) noexcept;
	Control(const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const Control&) = default;														
	Control(Control&&) = default;															
	Control& operator=(const Control&) = default;											
	Control& operator=(Control&&) = default;												

public:

	std::list<Control*> Controls;

	virtual ~Control() noexcept(false);																		
	void Dispose() override;

	void OnActivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnClickSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnDeactivateSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnGotFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnLostFocusSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnKeyDownSet(const std::function<void(Object*, KeyEventArgs*) >& callback) noexcept;
	void OnKeyPressSet(const std::function<void(Object*, KeyPressEventArgs*) >& callback) noexcept;
	void OnKeyUpSet(const std::function<void(Object*, KeyEventArgs*) >& callback) noexcept;
	void OnMouseClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseDownSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseEnterSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseHoverSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseLeaveSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;
	void OnMouseLeftDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseMoveSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseRightDoubleClickSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseUpSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnMouseWheelSet(const std::function<void(Object*, MouseEventArgs*)>& callback) noexcept;
	void OnPaintSet(const std::function<void(Object*, PaintEventArgs*)>& callback) noexcept;
	void OnVisibleChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	bool HasChildren() const noexcept;
	Size CalculateSizeByFont() noexcept;
	bool IsEnabled() const noexcept override;
	Window* GetWindow() noexcept;
	Padding GetMargin() const noexcept;
	Control* GetByTabIndex(const int& index) noexcept;
	bool IsTabSelected() const noexcept;
	const std::string& GetText() const noexcept;
	void SetText(const std::string& text) noexcept;
	Control* GetPreviousControl() noexcept;
	Control* GetNextControl() noexcept;
	Control* GetById(unsigned int id) noexcept;
	Control* GetByHandle(const IntPtr p) noexcept;
	int GetTabIndex() const noexcept;
	void SetTabIndex(const int& index) noexcept;
	void SetFont(Font font) noexcept override;
	void EnableTabStop() noexcept;
	void DisableTabStop() noexcept;
	bool IsTabStop() const noexcept;
};