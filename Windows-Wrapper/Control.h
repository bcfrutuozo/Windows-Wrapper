#pragma once

#include "Base.h"
#include "Common.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"
#include "EventHandler.h"
#include "MouseEventArgs.h"

#include <memory>
#include <functional>

class Window;

class Control : public IHandle, public Base
{
	friend class Menu;
	friend class MenuRadioItem;

private:

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

	void OnClickSet(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseClickSet(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseDoubleClick(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseDown(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseEnter(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseHover(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseLeave(const std::function<void(Control* const c, EventArgs* const e)>& callback) noexcept;
	void OnMouseMove(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseUp(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;
	void OnMouseWheel(const std::function<void(Control* const c, MouseEventArgs* const e)>& callback) noexcept;

	const std::string& GetText() const noexcept;
};