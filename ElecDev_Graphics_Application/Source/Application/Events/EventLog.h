#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include <memory>

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

class EventLog
{

public:

	template <class EventType, class ... Args>
	inline static void log(const Args&... args)
	{
		events.emplace_back(std::make_unique<EventType>(args...));
	}

	inline static void logNotify(LumenEventID ID, const std::string& msg = "")
	{
		notifyEvents.emplace_back(std::make_unique<NotifyEvent>(ID, msg));
	}

	inline static void logMouseMove(const glm::vec2& mousePositionPixels, LumenEventID ID)
	{
		// If event already exists update the existing data.
		if (mouseMove)
		{
			mouseMove->ID = ID;
			mouseMove->mousePosition = mousePositionPixels;
		}
		else
		{
			mouseMove = std::make_unique<MouseMoveEvent>(mousePositionPixels, ID);
		}
	}

	inline static void logMouseDrag(const glm::vec2& init, const glm::vec2& current, const glm::vec2 delta, LumenEventID ID)
	{
		// If event already exists update the existing data.
		if (mouseDrag)
		{
			mouseDrag->initialPosition = init;
			mouseDrag->currentFrameDelta += delta;
			mouseDrag->mousePosition = current;
			mouseDrag->ID = ID;
		}
		else
		{
			mouseDrag = std::make_unique<MouseDragEvent>(init, current, delta, ID);
		}
	}

	inline static void logMouseScroll(const glm::vec2& mousePositionPixels, float yOffset, float xOffset, LumenEventID ID)
	{
		// If event already exists update the existing data.
		if (mouseScroll)
		{
			mouseScroll->ID = ID;
			mouseScroll->mousePosition = mousePositionPixels;
			mouseScroll->xOffset = xOffset;
			mouseScroll->yOffset = yOffset;
		}
		{
			mouseScroll = std::make_unique<MouseScrollEvent>(mousePositionPixels, yOffset, xOffset, ID);
		}
	}

	// Setup the event log.
	inline static void init() 
	{
		events.reserve(10);
		notifyEvents.reserve(10);
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
