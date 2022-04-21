#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include <queue>

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

class EventLog
{

public:

	// Log any type of event.
	template <class EventType, class ... Args>
	inline static void log(const Args&... args)
	{
		events.emplace_back(std::make_unique<EventType>(args...));
	}

	inline static void logNotify(LumenEventID ID, const std::string& msg = "")
	{
		notifyEvents.emplace_back(NotifyEvent(ID, msg));
	}

	inline static void logMouseMove(const glm::vec2& mousePositionPixels, LumenEventID ID)
	{
		mouseMove.ID = ID | EventType_MouseMove;
		mouseMove.mousePosition = mousePositionPixels;
		mouseMoveOccurredFlag = true;
	}

	inline static void logMouseDrag(const glm::vec2& init, const glm::vec2& current, const glm::vec2 delta, LumenEventID ID)
	{
		// On first mouse drag event the delta has to be reset.
		if (!mouseDragOccurredFlag)
			mouseDrag.currentFrameDelta = {0.f, 0.f};

		mouseDrag.ID = ID | EventType_MouseDrag;
		mouseDrag.initialPosition = init;
		mouseDrag.currentFrameDelta += delta;
		mouseDrag.mousePosition = current;
		mouseDragOccurredFlag = true;
	}

	inline static void logMouseScroll(const glm::vec2& mousePositionPixels, float yOffset, float xOffset, LumenEventID ID)
	{
		mouseScroll.ID = ID | EventType_MouseScroll;
		mouseScroll.mousePosition = mousePositionPixels;
		mouseScroll.xOffset = xOffset;
		mouseScroll.yOffset = yOffset;
		mouseScrollOccurredFlag = true;
	}

	// Setup the event log.
	inline static void init() 
	{
		// ...
	}
	
	// Clears all of the events from the event log.
	inline static void clear() 
	{
		// Clear queues.
		events.clear();
		notifyEvents.clear();

		// Reset flags.
		mouseMoveOccurredFlag = false;
		mouseScrollOccurredFlag = false;
		mouseDragOccurredFlag = false;
	}

	// Getters.
	inline static std::deque<std::unique_ptr<Event>>& getEvents()	{ return events; }
	inline static std::deque<NotifyEvent>& getNotifyEvents()		{ return notifyEvents; }
	inline static MouseMoveEvent& getMouseMove()					{ return mouseMove; }
	inline static MouseScrollEvent& getMouseScroll()				{ return mouseScroll; }
	inline static MouseDragEvent& getMouseDrag()					{ return mouseDrag; }
	inline static bool mouseMoveOccurred()							{ return mouseMoveOccurredFlag; }
	inline static bool mouseScrollOccurred()						{ return mouseScrollOccurredFlag; }
	inline static bool mouseDragOccurred()							{ return mouseDragOccurredFlag; }

private:

	// Container for general (polymorphic) events.
	inline static std::deque<std::unique_ptr<Event>> events;

	// Store the notify events.
	inline static std::deque<NotifyEvent> notifyEvents;

	// These events are kept seperate, since we only want to 
	// handle one of them per frame and they are likely to fire 
	// more than once per frame.
	inline static MouseMoveEvent mouseMove		= MouseMoveEvent({ 0.f, 0.f }, 0);
	inline static MouseScrollEvent mouseScroll	= MouseScrollEvent({ 0.f, 0.f }, 0.f, 0.f, 0);
	inline static MouseDragEvent mouseDrag		= MouseDragEvent();

	// Flags for if the seperate events occured.
	inline static bool mouseMoveOccurredFlag = false;
	inline static bool mouseScrollOccurredFlag = false;
	inline static bool mouseDragOccurredFlag = false;

	// No instance.
	EventLog();
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
