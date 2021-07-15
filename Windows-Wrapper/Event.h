#pragma once

#include <string>
#include <optional>
#include <functional>
#include <vector>
#include <map>

class IEvent
{
public:

	virtual const unsigned int GetId() const = 0;
	virtual const std::string& GetName() const = 0;
};

template<typename> class Event;

template<typename R, typename ...Args>
class Event<R(Args...)> : public IEvent
{

private:

	unsigned int m_Id;
	std::string m_Name;
	std::function<R(Args...)> const m_Callback;

public:

	explicit Event(const std::string& name, const std::function<R(Args...)> &callback)
		:
		m_Name(name),
		m_Callback(callback)
	{
		// Automatically generate an incremental Id per callback function
		static unsigned int m_IdHandler = 0;
		m_Id = m_IdHandler++;
	}

	~Event()
	{	}

	virtual const unsigned int GetId() const override
	{
		return m_Id;
	}

	virtual const std::string& GetName() const override
	{
		return m_Name;
	}

	R Trigger(Args... a)
	{
		return m_Callback(a...);
	}
};

class EventDispatcher
{
private:

	std::map<std::string, std::vector<IEvent*>> m_List;

public:

	EventDispatcher();
	~EventDispatcher();

	void Register(IEvent* event);
	void Unregister(const std::string& name);

	template<typename... Args>
	void Dispatch(const std::string& name, Args... a)
	{
		auto it_eventList = m_List.find(name);

		if (it_eventList == m_List.end())
		{
			return;
		}

		for (auto ie : it_eventList->second)
		{
			if (Event<Args...>* event = dynamic_cast<Event<Args...>*>(ie))
			{
				return event->trigger(a...);
			}
		}
	}
};