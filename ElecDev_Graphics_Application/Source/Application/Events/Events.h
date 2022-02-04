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

enum EventLayer 
{
	None				=	1 << 0,
	Undetermined		=	1 << 1,
	App					=	1 << 2,
	GUI					=	1 << 3,
	Engine				=	1 << 4
};

enum EventType
{
	// Mouse events.
	MousePress			=	1 << 0,
	MouseRelease		=	1 << 1,
	MouseScroll			=	1 << 2,
	MouseMove			=	1 << 3,
	// Mouse button states.
	MouseButtonLeft		=	1 << 4,
	MouseButtonRight	=	1 << 5,
	MouseButtonMiddle	=	1 << 6,

	// Key events.
	KeyPress			=	1 << 7,
	KeyRelease			=	1 << 8,
	KeyRepeat			=	1 << 9,

	// Window events.
	WindowResize		=	1 << 10,

	// Application specific events.
	FileDrop			=	1 << 11,

	// Engine specific events.
	Focus				=	1 << 12,
	Defocus				=	1 << 13,
	Hover				=	1 << 14,
	Dehover				=	1 << 15,

	// GUI specific events.
	// These are currently handled by ImGUI.
	// Could be a good idea to make Lumen
	// handle them.

};

// Check if an ID contains a specific type.
bool operator==(uint64_t id, EventType eventType);
// Check if an ID does not contain a specific type.
bool operator!=(uint64_t id, EventType eventType);

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
	// Destructor (for polymorphic type).
	virtual ~Event() = default;

protected:

	// Constructor that sets the ID of the event.
	// This is a protected type to ensure that an 
	// 'Event' object is not created.
	Event(uint64_t ID);

private:

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

	// Returns the position of the mouse event in GLFW pixels.
	glm::vec2& getPositionGLFW();

protected:

	// Constructor that sets the mouse position.
	// This is a protected type to ensure that an 
	// 'MouseEvent' object is not created.
	MouseEvent(glm::vec2& positionPixels, uint64_t ID);

private:

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
	MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t ID);
};

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

class MouseMoveEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t ID);
};

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

class MouseScrollEvent : public MouseEvent
{

public:

	// Constructor with mouse position.
	MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t ID);

	// Getter for the amount of scrolling.
	float getYOffset();

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
	KeyEvent(int key, uint64_t ID);

	// Get the key associated with the event.
	int getKey();

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
	WindowResizeEvent(glm::vec2& windowSize);

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
//  Engine Specific Events.																														//
//==============================================================================================================================================//



//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//