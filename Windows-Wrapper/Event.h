#pragma once

#include <string>
#include <functional>
#include <map>

class Base;

class IEvent
{
public:

	virtual const std::string& GetName() const noexcept = 0;
};

template<typename ...Args>
class Event : public IEvent
{

private:

	std::string m_Name;
	std::function<void(Args...)> const m_Callback;

public:

	explicit Event(const std::string& name, const std::function<void(Args...)>& callback)
		:
		m_Callback(callback),
		m_Name(name)
	{

	}

	virtual const std::string& GetName() const noexcept override { return m_Name; }
	void Trigger(Args... a) { assert(m_Callback != nullptr); m_Callback(a...); 	}
};

class EventDispatcher
{
private:

	std::map<std::string, IEvent*> m_List;

public:

	EventDispatcher();
	~EventDispatcher();

	void Register(IEvent* event) noexcept;

	template<typename ...Args>
	void Dispatch(const std::string& name, Args... a)
	{
		if (const auto event = dynamic_cast<Event<Args...>*>(m_List[name]))
		{
			event->Trigger(a...);
		}
	}

	template<>
	void Dispatch(const std::string& name, Base& b)
	{
		if (const auto event = dynamic_cast<Event<Base&>*>(m_List[name]))
		{
			event->Trigger(b);
		}
	}
};