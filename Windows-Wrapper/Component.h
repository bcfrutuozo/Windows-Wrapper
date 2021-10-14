#pragma once

#include "IComponent.h"
#include "ISite.h"
#include "IContainer.h"
#include "EventArgs.h"
#include "EventDispatcher.h"

#include <string>

class Component : public IComponent
{
protected:

	EventDispatcher Events;
	inline bool CanRaiseEvents();
	void Dispose(bool disposing);
	void Dispatch(const std::string& name, EventArgs* e);

public:

	Component();
	virtual ~Component() noexcept(false);

	bool IsDesignMode();
	virtual ISite* GetSite();
	virtual void SetSite(ISite* const site);
	virtual void Dispose() override;
	virtual std::string ToString() const noexcept override;
	IContainer* GetContainer() const noexcept;
};