//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "EventLog.h"

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

EventLog::EventLog()
{
	events.reserve(20);
}

void EventLog::log(Event& event)
{
	uint64_t eventID = event.getID();

	// Specific mouse events.
	if (eventID == EventType::MouseMove)
	{
		mouseMove = std::make_unique<MouseMoveEvent>(std::move(dynamic_cast<MouseMoveEvent&>(event)));
	}
	else if (eventID == EventType::MouseScroll)
	{
		mouseScroll = std::make_unique<MouseScrollEvent>(std::move(dynamic_cast<MouseScrollEvent&>(event)));
	}
	// General events.
	else
	{
		events.emplace_back(std::make_unique<Event>(std::move(event)));
	}
}

void EventLog::clear()
{
	mouseMove = nullptr;
	mouseScroll = nullptr;
	events.clear();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//