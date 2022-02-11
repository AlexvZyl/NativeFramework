#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include <memory>

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

// This is used to log the events as they happen per frame.
class EventLog
{

public:

	// Constructor.
	EventLog();

	// Log an event.
	template <typename EventType>
	void log(Event& event);

	// Clears all of the events from the event log.
	void clear();

private:

	// Only the application needs to have access to dispatch 
	// the specific events.
	friend class Application;

	// Store all of the events that occurred.
	std::vector<std::unique_ptr<Event>> events;

	// Keep mouse move and scroll events seperate so that multiple
	// events that basically do the same thing do not get handled
	// more than once.
	std::unique_ptr<MouseMoveEvent> mouseMove = nullptr;
	std::unique_ptr<MouseScrollEvent> mouseScroll = nullptr;
};

//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template <typename EventType>
void EventLog::log(Event& event)
{
	// Mouse move.
	if (typeid(EventType) == typeid(MouseMoveEvent))
		mouseMove = std::make_unique<MouseMoveEvent>(std::move(dynamic_cast<MouseMoveEvent&>(event)));

	// Mouse scroll.
	else if (typeid(EventType) == typeid(MouseScrollEvent))
		mouseScroll = std::make_unique<MouseScrollEvent>(std::move(dynamic_cast<MouseScrollEvent&>(event)));

	// General events.
	else 
		events.emplace_back(std::make_unique<EventType>(std::move(dynamic_cast<EventType&>(event))));
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//