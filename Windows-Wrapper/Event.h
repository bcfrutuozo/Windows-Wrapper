#pragma once

#include "Event.h"

#include <string>
#include <optional>
#include <functional>
#include <vector>
#include <map>

class IEvent
{
public:

	virtual const unsigned int GetId() const = 0;
};

template<typename ...Args>
class Event : public IEvent
{

private:

	unsigned int m_Id;
	std::function<void(Args...)> const m_Callback;

public:

	explicit Event(const std::function<void(Args...)>& callback)
		:
		m_Id(0),
		m_Callback(callback)
	{

	}

	~Event()
	{	}

	const unsigned int GetId() const override
	{
		return m_Id;
	}

	void Trigger(Args... a)
	{
		assert(m_Callback != nullptr);
		m_Callback(a...);
	}
};

class EventDispatcher
{
private:

	std::map<unsigned int, IEvent*> m_List;

public:

	EventDispatcher();
	~EventDispatcher();

	unsigned int Register(IEvent* event);
	void Unregister(unsigned int id);

	template<typename ...Args>
	void Dispatch(unsigned int id, Args... a)
	{
		auto it_eventList = m_List.find(id);

		if (it_eventList == m_List.end())
		{
			return;
		}

		if (Event<Args...>* event = dynamic_cast<Event<Args...>*>(it_eventList->second))
		{
			event->Trigger(a...);
		}
	}
};