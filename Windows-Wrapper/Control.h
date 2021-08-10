#pragma once

#include "Base.h"
#include "Color.h"
#include "Common.h"
#include "Exception.h"
#include "IHandle.h"
#include "Size.h"
#include "Padding.h"
#include "Event.h"
#include "EventHandler.h"
#include "KeyEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyPressEventArgs.h"
#include "MessageMapper.h"
#include "CancelEventArgs.h"
#include "OnClosedEventArgs.h"
#include "OnClosingEventArgs.h"

#include <memory>
#include <functional>

// Error exception helper macro
#define CTL_EXCEPT( hr ) Control::HRException( __LINE__,__FILE__,(hr) )
#define CTL_LAST_EXCEPT() Control::HRException( __LINE__,__FILE__,GetLastError() )

class Window;

class Control : public IHandle, public Base
{
	friend class Window;
	friend class Menu;
	friend class MenuRadioItem;
	friend class Button;

private:

	bool m_IsMouseOver = false;
	bool m_IsClicking = false;

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;

	// Used by some controls to trigger certain rules updates
	virtual void OnInternalUpdate(Control* const sender, EventArgs* const args) {};
	static HBRUSH CreateGradientBrush(Color top, Color bottom, LPNMCUSTOMDRAW item) noexcept;

protected:

	std::string Text;
	Size Size;
	Point Location;
	Control* Parent;
	std::vector<std::shared_ptr<Control>> Controls;
	EventDispatcher Events;
	Color m_ForeColor;
	Color m_BackgroundColor;
	static MessageMapper Mapper;
	HBRUSH m_Brush;
	Padding Margin;
	Padding Padding;

	// Forces the implementation and call on individual childs because each WinApi control
	// has it own creation method.
	// ALL DERIVED CLASSES MUST CALL THIS METHOD ON ITS CONSTRUCTOR
	virtual void Initialize() noexcept = 0;

	template<typename T, typename... Args>
	T& Create(Args... a)
	{
		Controls.push_back(std::move(std::make_shared<T>(a...)));
		return dynamic_cast<T&>(*Controls.back());
	}

	// Going to think a proper way to handle events
	// Afterall, only EventArgs type are allowed instead of multiple types
	template<typename ...Args>
	void Dispatch(const std::string& event, Args... args)
	{
		// Pass the event name, the control which will compose the "sender" parameter in the event
		// and the arguments which will be an EventArgs type
		Events.Dispatch(event, this, args...);
	}

	Control* GetById(unsigned int id) noexcept;
	Control* GetByHandle(const IntPtr p) noexcept;
	const unsigned int GetId() const noexcept;
	void SetMouseOverState(bool state) noexcept;
	void SetClickingState(bool state) noexcept;

public:

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

	void Delete();

	void OnActivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnClickSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnDeactivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnGotFocusSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnLostFocusSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnKeyDownSet(const std::function<void(Control* const c, KeyEventArgs* const e) >& callback) noexcept;
	void OnKeyPressSet(const std::function<void(Control* const c, KeyPressEventArgs* const e) >& callback) noexcept;
	void OnKeyUpSet(const std::function<void(Control* const c, KeyEventArgs* const e) >& callback) noexcept;
	void OnMouseClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseDownSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseEnterSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseHoverSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseLeaveSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseLeftDoubleClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseMoveSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseRightDoubleClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseUpSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseWheelSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnVisibleChangedSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;

	void SetForeColor(const Color& color) noexcept;
	virtual void SetBackgroundColor(const Color& color) noexcept;
	const std::string& GetText() const noexcept;
	Window* GetWindow() noexcept;
	bool IsMouseOver() const noexcept;
	bool IsClicking() const noexcept;

	// Control Exception
	class ControlException : public Exception
	{
	public:

		static const std::string& TranslateErrorCode(HRESULT hr) noexcept;
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
		const std::string& GetErrorDescription() const noexcept;
	};
};