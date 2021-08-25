#pragma once

#include "Base.h"
#include "IComponent.h"
#include "Event.h"

class Component : public Base, public IComponent
{
	friend class Control;
	friend class Window;
	friend class ToolStrip;
	friend class Button;
	friend class ProgressBar;
	friend class TextBox;

private:

	EventDispatcher Events;

protected:

	inline bool CanRaiseEvents();
	void Dispose(bool disposing) noexcept;
	
	// Going to think a proper way to handle events
	// Afterall, only EventArgs type are allowed instead of multiple types
	template<typename ...Args>
	void Dispatch(const std::string& event, Args... args)
	{
		// Pass the event name, the control which will compose the "sender" parameter in the event
		// and the arguments which will be an EventArgs type
		if (!IsDisposed)
		{
			Events.Dispatch(event, dynamic_cast<Control*>(this), args...);
		}
	}

public:

	Component();
	~Component();

	bool IsDesignMode();
	virtual ISite* GetSite();
	virtual void SetSite(ISite* const site);
	virtual void Dispose() override;
	inline virtual const std::string ToString() const noexcept override;
	IContainer* GetContainer() const noexcept;
};