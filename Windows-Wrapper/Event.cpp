#include "Event.h"

EventDispatcher::EventDispatcher()
{

}

EventDispatcher::~EventDispatcher()
{
	for (auto el : m_List)
	{
		for (auto e : el.second)
		{
			delete e;
		}
	}
}

void EventDispatcher::Register(IEvent* event)
{
	if (event)
	{
		m_List[event->GetName()].push_back(event);
	}

}

void EventDispatcher::Unregister(const std::string& name)
{
	auto e = m_List.find(name);

	if (e == m_List.end())
	{
		return;
	}

	m_List.erase(name);
}