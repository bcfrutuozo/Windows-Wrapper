#pragma once

#include "IComponent.h"
#include "EventDispatcher.h"

class Component : public IComponent
{
	friend class WinAPI;
	friend class Control;
	friend class Window;
	friend class ToolStrip;
	friend class Button;
	friend class ProgressBar;
	friend class TextBox;
	friend class ListControl;

private:

	EventDispatcher Events;
	bool Disposed;
	bool Disposing;

protected:

	inline bool CanRaiseEvents();
	void Dispose(bool disposing);
	void Dispatch(const std::string& name, EventArgs* e);

public:

	Component();
	virtual ~Component() noexcept(false);

	bool IsDisposed() const noexcept;
	bool IsDisposing() const noexcept;
	bool IsDesignMode();
	virtual ISite* GetSite();
	virtual void SetSite(ISite* const site);
	virtual void Dispose() override;
	inline virtual const std::string ToString() const noexcept override;
	IContainer* GetContainer() const noexcept;
};