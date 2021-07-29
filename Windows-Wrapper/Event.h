#pragma once

#include "Base.h"

#include <string>
#include <functional>
#include <map>
#include <memory>

class IEvent
{
public:

	virtual const std::string& GetName() const noexcept = 0;
};

template<typename ...Args>
class Event : public IEvent, public Base
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

	virtual ~Event() = default;

	virtual const std::string& GetName() const noexcept override { return m_Name; }
	void Trigger(Args... a) { assert(m_Callback != nullptr); m_Callback(a...); }
};

class EventDispatcher : public Base
{
private:

	std::map<std::string, std::unique_ptr<IEvent>> m_List;

public:

	EventDispatcher();
	virtual ~EventDispatcher();

	void Register(std::unique_ptr<IEvent>&& event) noexcept;
	void Unregister(const std::string& name) noexcept;
	bool Contains(const std::string& name) noexcept;
	void Clear() noexcept;

	template<typename ...Args>
	void Dispatch(const std::string& name, Args... a)
	{
		if (const auto event = dynamic_cast<Event<Args...>*>(m_List[name].get()))
		{
			event->Trigger(a...);
		}
	}
};