#pragma once

#include "Object.h"
#include "IEvent.h"

#include <functional>

class Component;

template<typename TArgs>
class Event : public IEvent, public Object
{
private:

	std::string m_Name;
	std::function<void(Object*, TArgs)> const m_Callback;

public:

	explicit Event(const std::string& name, const std::function<void(Object*, TArgs)>& callback)
		:
		m_Name(name),
		m_Callback(callback)
	{

	}

	virtual ~Event() = default;

	virtual const std::string& GetName() const noexcept override { return m_Name; }
	void Trigger(Object* sender, EventArgs* e) const noexcept { assert(m_Callback != nullptr); m_Callback(sender, (TArgs)e); }
};