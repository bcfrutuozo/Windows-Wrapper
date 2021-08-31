#pragma once

#include "Common.h"
#include "WinAPI.h"
#include "Exception.h"
#include "Color.h"
#include "Size.h"
#include "Padding.h"
#include "EventHandler.h"
#include "KeyEventHandler.h"
#include "KeyPressEventHandler.h"
#include "MouseEventHandler.h"
#include "KeyEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyPressEventArgs.h"
#include "CancelEventArgs.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"
#include "Collection.h"

#include <memory>
#include <functional>

// Error exception helper macro
#define CTL_EXCEPT( hr ) Control::HRException( __LINE__,__FILE__,(hr) )
#define CTL_LAST_EXCEPT() Control::HRException( __LINE__,__FILE__,GetLastError() )

class Control : public WinAPI
{
	friend class Window;
	friend class Button;
	friend class ToolStrip;
	friend class TextBox;

private:

	// Tabulation fields
	unsigned int m_TabIndex;
	static unsigned int m_IncrementalTabIndex;
	std::string Name;

	// Variable to check if control is tab selected
	bool m_IsTabSelected;

	void OnFocusEnter_Impl(HWND hwnd, HWND hwndOldFocus) noexcept override;
	void OnFocusLeave_Impl(HWND hwnd, HWND hwndNewFocus) noexcept override;
	void OnKeyDown_Impl(HWND hwnd, unsigned int vk, int cRepeat, unsigned int flags) noexcept override;
	void OnMouseLeftDown_Impl(HWND hwnd, int x, int y, unsigned int keyFlags) noexcept override;
	void OnNextDialogControl_Impl(HWND hwnd, HWND hwndSetFocus, bool fNext) noexcept override;

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
	EventHandler* OnVisibleChanged;

protected:

	Point Location;
	Control* Parent;
	std::string Text;
	Size m_Size;
	Color m_ForeColor;
	Color m_BackgroundColor;
	Padding m_Margin;
	Padding m_Padding;
	Font m_Font;
	unsigned int m_MinSize;

	// Forces the implementation and call on individual childs because each WinApi control
	// has it own creation method.
	// ALL DERIVED CLASSES MUST CALL THIS METHOD ON ITS CONSTRUCTOR
	virtual void Initialize() = 0;

	template<typename T, typename... Args>
	T* Create(Args... a)
	{
		T* item = new T(a...);
		Controls.Add(item);
		return item;
	}

public:

	class ControlCollection : public Collection<Control>
	{
	public:

		ControlCollection(Control* owner);
	};

	ControlCollection Controls;

	Control() noexcept;
	Control(Control* parent, const std::string& text) noexcept;
	Control(Control* parent, const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const std::string& text) noexcept;
	Control(const std::string& text, int width, int height, int x, int y) noexcept;
	Control(const Control&) = default;														// Copy constructor
	Control(Control&&) = default;															// Move constructor
	Control& operator=(const Control&) = default;											// Copy assignment constructor
	Control& operator=(Control&&) = default;												// Move assignment constructor
	virtual ~Control();																		// Destructor

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
	void OnVisibleChangedSet(const std::function<void(Object*, EventArgs*)>& callback) noexcept;

	bool HasChildren() const noexcept;
	Size CalculateSizeByFont() noexcept;
	bool IsEnabled() const noexcept;
	Window* GetWindow() noexcept;
	Size GetSize() const noexcept;
	Padding GetMargin() const noexcept;
	Control* GetByTabIndex(const unsigned int& index) noexcept;
	void SetForeColor(const Color& color) noexcept;
	void SetBackgroundColor(const Color& color) noexcept;
	const std::string& GetText() const noexcept;
	Control* GetPreviousControl() noexcept;
	Control* GetNextControl() noexcept;
	Control* GetById(unsigned int id) noexcept;
	Control* GetByHandle(const IntPtr p) noexcept;
	int GetTabIndex() const noexcept;
	void SetTabIndex(const unsigned int& index) noexcept;
	Font GetFont() const noexcept;
	void SetFont(Font font) noexcept;

	// Control Exception
	class ControlException : public Exception
	{
	public:

		static const std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	// HRException
	class HRException : public Exception
	{
	private:

		HRESULT hr;

	public:

		HRException(int line, const char* file, HRESULT hr) noexcept;

		const char* what() const noexcept override;
		const char* GetType() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		const std::string GetErrorDescription() const noexcept;
	};
};