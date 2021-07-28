#pragma once

#include "Base.h"
#include "Common.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"
#include "MouseEventArgs.h"

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

	Control();
	Control(Control* parent, const std::string& text);
	Control(Control* parent, const std::string& text, int width, int height, int x, int y);
	Control(const std::string& text);
	Control(const std::string& text, int width, int height, int x, int y);
	Control(Control&& other) = default;
	Control& operator=(const Control&) = default;
	Control& operator=(Control&&) = default;
	virtual ~Control();

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

	virtual void OnClick(Control* const sender, EventArgs* const args)
	{
		
	}

	virtual void OnMouseClick(Control* const sender, MouseEventArgs* const args)
	{

	}

	virtual void OnMouseDoubleClick(Control* const sender, MouseEventArgs* const args)
	{

	}

	virtual void OnMouseDown(Control* const sender, MouseEventArgs* const args)
	{

	}

	virtual void OnMouseEnter(Control* const sender, EventArgs* const args)
	{

	}

	virtual void OnMouseHover(Control* const sender, EventArgs* const args)
	{

	}

	virtual void OnMouseLeave(Control* const sender, EventArgs* const args)
	{

	}

	virtual void OnMouseMove(Control* const sender, MouseEventArgs* const args)
	{

	}

	virtual void OnMouseUp(Control* const sender, MouseEventArgs* const args)
	{

	}

	virtual void OnMouseWheel(Control* const sender, MouseEventArgs* const args)
	{

	}

	const std::string& GetText() const noexcept;
};