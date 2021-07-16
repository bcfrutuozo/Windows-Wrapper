#include "Common.h"
#include "Event.h"

#include <sstream>

EventDispatcher::EventDispatcher()
{

}

EventDispatcher::~EventDispatcher()
{
	for (auto el : m_List)
	{
		delete el.second;
	}
}

unsigned int EventDispatcher::Register(IEvent* event)
{
	// Automatically generate an incremental Id per callback function
	// Consider 0 = NULL meaning no function handler.That's why the value is set to 1 as default.
	static unsigned int m_IdHandler = 1;
	
	if (event)
	{
		
		m_List[m_IdHandler++] = event;
	}

	return m_IdHandler;
}

void EventDispatcher::Unregister(unsigned int id)
{
	auto e = m_List.find(id);

	if (e == m_List.end())
	{
		return;
	}

	m_List.erase(id);
}