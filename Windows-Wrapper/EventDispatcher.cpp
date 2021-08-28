#include "EventDispatcher.h"

EventDispatcher::EventDispatcher(Component* parent)
	:
	Parent(parent)
{

}

EventDispatcher::~EventDispatcher()
{
	
}

void EventDispatcher::Register(IEvent* event) noexcept
{
	if (event != nullptr)
	{
		// First remove a already registered event with the same name
		Unregister(event->GetName());

		// Register the event properly
		m_List[event->GetName()] = event;
	}
}

void EventDispatcher::Unregister(const std::string& name) noexcept
{
	auto const entry = m_List.find(name);

	// Remove event if it's registered
	if (entry != m_List.end())
	{
		m_List.erase(entry);
		// DO NOT DELETE Event pointer. It is handled inside the Control class destructor
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

void EventDispatcher::Dispatch(const std::string& name, Object* sender, EventArgs* e)
{
	if (const auto event = dynamic_cast<IEvent*>(m_List[name]))
	{
		event->Trigger(sender, e);
	}
}
