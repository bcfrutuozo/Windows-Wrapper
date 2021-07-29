#include "Common.h"
#include "Event.h"

#include <sstream>

EventDispatcher::EventDispatcher()
{

}

EventDispatcher::~EventDispatcher()
{

}

void EventDispatcher::Register(std::unique_ptr<IEvent>&& event) noexcept
{
	if (event != nullptr)
	{
		// First remove a already registered event with the same name
		Unregister(event->GetName());

		// Register the event properly
		m_List[event->GetName()] = std::move(event);
	}
}

void EventDispatcher::Unregister(const std::string& name) noexcept
{
	auto const entry = m_List.find(name);

	// Remove event if it's registered
	if (entry != m_List.end())
	{
		m_List.erase(entry);
	}
}

bool EventDispatcher::Contains(const std::string& name) noexcept
{
	if (m_List.count(name) == 0)
	{
		return false;
	}
	
	return true;
}

void EventDispatcher::Clear() noexcept
{
	m_List.clear();
}