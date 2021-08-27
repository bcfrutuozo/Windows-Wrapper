#pragma once

#include "Object.h"
#include "IComponent.h"
#include "EventDispatcher.h"

class Component : public Object, public IComponent
{
	friend class WinAPI;
	friend class Control;
	friend class Window;
	friend class ToolStrip;
	friend class Button;
	friend class ProgressBar;
	friend class TextBox;

private:

	EventDispatcher Events;
	bool Disposed;

protected:

	inline bool CanRaiseEvents();
	void Dispose(bool disposing) noexcept;
	void Dispatch(const std::string& name, EventArgs* e) noexcept;

public:

	Component();
	~Component();

	bool IsDisposed() const noexcept;
	bool IsDesignMode();
	virtual ISite* GetSite();
	virtual void SetSite(ISite* const site);
	virtual void Dispose() override;
	inline virtual const std::string ToString() const noexcept override;
	IContainer* GetContainer() const noexcept;
};