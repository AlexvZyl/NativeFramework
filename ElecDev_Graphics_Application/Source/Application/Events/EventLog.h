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

	template <class EventType, class ... Args>
	inline static void log(const Args&... args)
	{
		events.emplace_back(std::make_unique<EventType>(args...));
	}

	template <class ... Args>
	inline static void logNotify(const Args&... args)
	{
		notifyEvents.emplace_back(std::make_unique<NotifyEvent>(args...));
	}

	template <class ... Args>
	inline static void logMouseMove(const Args&... args) 
	{
		mouseMove = std::make_unique<MouseMoveEvent>(args...);
	}

	template <class ... Args>
	inline static void logMouseDrag(const Args&... args) 
	{
		// If event exists, increment delta.
		if (mouseDrag)
		{
			glm::vec2 delta = mouseDrag->currentFrameDelta;
			mouseDrag = std::make_unique<MouseDragEvent>(args...);
			mouseDrag->currentFrameDelta += delta;
		}
		else
		{
			mouseDrag = std::make_unique<MouseDragEvent>(args...);
		}
	}

	template <class ... Args>
	inline static void logMouseScroll(const Args&... args) 
	{
		mouseScroll = std::make_unique<MouseScrollEvent>(args...);
	}

	// Setup the event log.
	inline static void init() 
	{
		events.reserve(15);
	}
	
	// Clears all of the events from the event log.
	inline static void clear() 
	{
		events.clear();
		notifyEvents.clear();
		mouseMove = nullptr;
		mouseDrag = nullptr;
		mouseScroll = nullptr;
	}

	// Store all of the events that occurred.
	inline static std::vector<std::unique_ptr<Event>> events;

	// Store the notify events.
	inline static std::vector<std::unique_ptr<NotifyEvent>> notifyEvents;

	// These events are kept seperate, since we only want to 
	// handle one of them per frame.
	inline static std::unique_ptr<MouseMoveEvent> mouseMove = nullptr;
	inline static std::unique_ptr<MouseScrollEvent> mouseScroll = nullptr;
	inline static std::unique_ptr<MouseDragEvent> mouseDrag = nullptr;

private:

	EventLog();

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
