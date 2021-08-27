#pragma once

#include "Object.h"
#include "IEvent.h"
#include "EventArgs.h"

#include <map>

class Component;

class EventDispatcher : public Object
{
private:

	Component* Parent;
	std::map<std::string, IEvent*> m_List;

public:

	EventDispatcher(Component* parent);
	virtual ~EventDispatcher();

	void Register(IEvent* event) noexcept;
	void Unregister(const std::string& name) noexcept;
	bool Contains(const std::string& name) noexcept;
	void Clear() noexcept;

	// Solved the inheritance type conversion by passing Object* instead of child type
	void Dispatch(const std::string& name, Object* sender, EventArgs* e);
};

