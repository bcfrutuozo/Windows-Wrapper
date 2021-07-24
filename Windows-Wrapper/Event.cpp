#include "Common.h"
#include "Event.h"

#include <sstream>

EventDispatcher::EventDispatcher()
{

}

EventDispatcher::~EventDispatcher()
{
	for (const auto& e : m_List)
	{
		delete e.second;
	}
}

void EventDispatcher::Register(IEvent* event) noexcept
{
	if (event != nullptr)
	{
		m_List[event->GetName()] = event;
	}
}