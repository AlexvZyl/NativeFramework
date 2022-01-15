#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <stdint.h>
#include "External/GLM/glm.hpp"

//==============================================================================================================================================//
//  Event types.																																	//
//==============================================================================================================================================//

enum EventType
{
	// Mouse events.
	MousePress			=	1 << 0,
	MouseRelease		=	1 << 1,
	MouseButonLeft		=	1 << 2,
	MouseButtonRight	=	1 << 3,
	MouseButtonMiddle	=	1 << 4,
	MouseScroll			=	1 << 5,
	MouseMove			=	1 << 6,

	// Key events.
	KeyPress			=	1 << 7,
	KeyRelease			=	1 << 8,

	// Window events.
	WindowResize		=	1 << 9,
};

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

class Event
{

public:

	// Called when the event has been handled.
	void handled() 
	{
		m_handled = true;
	}

	// Checks if the event has been handled.
	bool isHandled()
	{
		return m_handled;
	}

	// Gets the ID of the event.
	uint64_t getID() 
	{
		return m_eventID;
	}

private:

	// Classes that can use 'Event'.
	friend class MouseEvent;
	friend class KeyEvent;
	friend class WindowResizeEvent;

	// Constructor that sets the ID of the event.
	Event(uint64_t eventID) 
	{ 
		m_eventID = eventID; 
	}

	// ID describing the event.
	uint64_t m_eventID;

	// Has the event been handled?
	bool m_handled = false;

};

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

// --------------------- //
//  M O U S E   B A S E  //
// --------------------- //

class MouseEvent : public Event
{

public:

	// Returns the position of the mouse event in pixels.
	glm::vec2& getPositionPixels() 
	{
		return m_mousePositionPixels;
	}

private:

	// Classes that can use 'MouseEvent'.
	friend class MouseButtonEvent;
	friend class MouseMoveEvent;
	friend class MouseScrollEvent;

	// Constructor that sets the mouse position.
	MouseEvent(glm::vec2& positionPixels, uint64_t eventID) 
		: Event(eventID) 
	{
		m_mousePositionPixels = positionPixels;
	};

	// The postion of the mouse event.
	glm::vec2 m_mousePositionPixels = { 0.f, 0.f };
};

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

class MouseButtonEvent : public MouseEvent 
{

public:

	// Constructor with mouse position.
	MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t eventID)
		: MouseEvent(mousePositionPixels, eventID)
	{}

};

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

class MouseMoveEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t eventID = EventType::MouseMove)
		: MouseEvent(mousePositionPixels, eventID | EventType::MouseMove)
	{}

};

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

class MouseScrollEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t eventID = EventType::MouseScroll)
		: MouseEvent(mousePositionPixels, eventID | EventType::MouseScroll)
	{
		m_yOffset = yOffset;
	}

private:

	// How much the mouse wheel scrolled.
	float m_yOffset = 0;

};

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

class KeyEvent : public Event
{

public:

	// Constructor.
	KeyEvent(int key, uint64_t eventID)
		: Event(eventID)
	{
		m_key = key;
	};

	// Get the key associated with the event.
	int getKey() 
	{
		return m_key;
	}

private:

	// Key associated with the event.
	int m_key;

};

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

class WindowResizeEvent : public Event
{

public:

	// Constructor.
	WindowResizeEvent(glm::vec2& windowSize, uint64_t eventID = EventType::WindowResize)
		: Event(eventID)
	{
		m_windowSize = windowSize;
	}

private:

	// The new size of the window.
	glm::vec2 m_windowSize = { 0.f, 0.f };

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
