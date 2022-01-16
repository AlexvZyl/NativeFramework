#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "External/GLM/glm.hpp"

//==============================================================================================================================================//
//  Event types.																																//
//==============================================================================================================================================//

enum EventType
{
	// Mouse events.
	MousePress			=	1 << 0,
	MouseRelease		=	1 << 1,
	MouseButtonLeft		=	1 << 2,
	MouseButtonRight	=	1 << 3,
	MouseButtonMiddle	=	1 << 4,
	MouseScroll			=	1 << 5,
	MouseMove			=	1 << 6,

	// Key events.
	KeyPress			=	1 << 7,
	KeyRelease			=	1 << 8,

	// Window events.
	WindowResize		=	1 << 9,

	// Serialisation events.
	FileDrop			=	1 << 10,
};

// Check if an ID contains a specific type.
bool operator==(uint64_t id, EventType eventType);

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

class Event
{

public:

	// Called when the event has been handled.
	void handled();

	// Checks if the event has been handled.
	bool isHandled();

	// Gets the ID of the event.
	uint64_t getID();

private:

	// Classes that can use 'Event'.
	// We do not want the user to be able to create 'Event' objects.
	friend class MouseEvent;
	friend class KeyEvent;
	friend class WindowResizeEvent;
	friend class FileDropEvent;

	// Constructor that sets the ID of the event.
	Event(uint64_t eventID);

	// Destructor.
	virtual ~Event() = default;

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
	glm::vec2& getPositionGLFW();

private:

	// Classes that can use 'MouseEvent'.
	// We do not want the user to be able to create 'MouseEvent' objects.
	friend class MouseButtonEvent;
	friend class MouseMoveEvent;
	friend class MouseScrollEvent;

	// Constructor that sets the mouse position.
	MouseEvent(glm::vec2& positionPixels, uint64_t eventID);

	// The postion of the mouse event.
	glm::vec2 m_mousePositionGLFW = { 0.f, 0.f };
};

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

class MouseButtonEvent : public MouseEvent 
{

public:

	// Constructor with mouse position.
	MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t eventID);
};

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

class MouseMoveEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t eventID = EventType::MouseMove);
};

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

class MouseScrollEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t eventID = EventType::MouseScroll);

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
	KeyEvent(int key, uint64_t eventID);

	// Get the key associated with the event.
	int getKey();

private:

	// Key associated with the event.
	int m_key;

};

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

// This might rather be handled differently, since ImGUI keeps track
// of window sizes.
class WindowResizeEvent : public Event
{

public:

	// Constructor.
	WindowResizeEvent(glm::vec2& windowSize, uint64_t eventID = EventType::WindowResize);

	// Get the resized window size.
	glm::vec2& getWindowSize();

private:

	// The new size of the window.
	glm::vec2 m_windowSize = { 0.f, 0.f };

};

//==============================================================================================================================================//
//  File drop event.																															//
//==============================================================================================================================================//

class FileDropEvent : public Event
{
public:

	// Constructor.
	FileDropEvent(std::vector<std::string>& paths);

	// Get the dropped files.
	std::vector<std::string>& getPaths();

private:

	// The path to the dropped files.
	std::unique_ptr<std::vector<std::string>> filePaths;
		
};

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

class EventLog
{

public:

	// Constructor.
	EventLog();

	// Log the event passed.
	void log(Event& event);

	// Clears all of the events from the event log.
	void clear();

	// Mouse events.
	std::unique_ptr<MouseButtonEvent> mousePressEvent = nullptr;
	std::unique_ptr<MouseButtonEvent> mouseReleaseEvent = nullptr;
	std::unique_ptr<MouseMoveEvent> mouseMoveEvent = nullptr;
	std::unique_ptr<MouseScrollEvent> mouseScrollEvent = nullptr;
	
	// Key events.
	std::vector<std::unique_ptr<KeyEvent>> keyPressEvents;
	std::vector<std::unique_ptr<KeyEvent>> keyReleaseEvents;

	// Window events.
	std::unique_ptr<WindowResizeEvent> windowResizeEvent = nullptr;

	// Serialisation events.
	std::unique_ptr<FileDropEvent> fileDropEvent = nullptr;
};

// State used throughout the application.
// Has to be global so that GLFW can use it for callbacks.
static EventLog eventLog;

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//