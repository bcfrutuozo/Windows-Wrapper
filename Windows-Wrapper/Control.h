#pragma once

#include "Base.h"
#include "Color.h"
#include "Common.h"
#include "Exception.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"
#include "EventHandler.h"
#include "KeyEventArgs.h"
#include "MouseEventArgs.h"
#include "KeyPressEventArgs.h"

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

private:

	// Fields for callback bindings to WinAPI
	unsigned int m_Id;
	static unsigned int m_CurrentIndex;

	// Used by some controls to trigger certain rules updates
	virtual void OnInternalUpdate(Control* const sender, EventArgs* const args) {};

protected:

	std::string Text;
	Size Size;
	Point Location;
	Control* Parent;
	std::vector<std::shared_ptr<Control>> Controls;
	EventDispatcher Events;

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
	const unsigned int GetId() const noexcept;

public:

	Control();
	Control(Control* parent, const std::string& text);
	Control(Control* parent, const std::string& text, int width, int height, int x, int y);
	Control(const std::string& text);
	Control(const std::string& text, int width, int height, int x, int y);
	Control(const Control&) = default;														// Copy constructor
	Control(Control&&) = default;															// Move constructor
	Control& operator=(const Control&) = default;											// Copy assignment constructor
	Control& operator=(Control&&) = default;												// Move assignment constructor
	virtual ~Control() = default;															// Destructor

	void Delete();

	void OnActivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnClickSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnDeactivateSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
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
	void OnShowSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;

	const std::string& GetText() const noexcept;
	Window* GetWindow() const noexcept;

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