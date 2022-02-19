#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "glm/glm.hpp"

//==============================================================================================================================================//
//  Event types.																																//
//==============================================================================================================================================//

enum EventType
{
	// Layer specific identifiers.
	EventType_Error				=	1 << 0,
	EventType_Application		=	1 << 1,

	// Mouse events.
	EventType_MousePress		=	1 << 2,
	EventType_MouseRelease		=	1 << 3,
	EventType_MouseScroll		=	1 << 4,
	EventType_MouseMove			=	1 << 5,
	// Mouse button states.
	EventType_MouseButtonLeft	=	1 << 6,
	EventType_MouseButtonRight	=	1 << 7,
	EventType_MouseButtonMiddle	=	1 << 8,

	// Key events.
	EventType_KeyPress			=	1 << 9,
	EventType_KeyRelease		=	1 << 10,
	EventType_KeyRepeat			=	1 << 11,
	// Key states.
	EventType_LeftCtrl			=	1 << 12,
	EventType_RightCtrl			=	1 << 13,
	EventType_LeftShift			=	1 << 14,
	EventType_RightShift		=	1 << 15,
	EventType_LeftAlt			=	1 << 16,
	EventType_RightAlt			=	1 << 17,

	// Window events.
	EventType_WindowResize		=	1 << 18,
	EventType_WindowMove		=	1 << 19,
	EventType_WindowClose		=	1 << 20,

	// Application specific events.
	EventType_FileDrop			=	1 << 21,

	// Layer events.
	EventType_Focus				=	1 << 22,
	EventType_Defocus			=	1 << 23,
	EventType_Hover				=	1 << 24,
	EventType_Dehover			=	1 << 25,
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
	void consume();
	// Checks if the event has been handled.
	bool isConsumed();
	// Destructor (for polymorphic type).
	virtual ~Event() = default;

	// ID describing the event.
	uint64_t ID;

protected:
	 
	// Constructor that sets the ID of the event.
	// This is a protected type to ensure that an 
	// 'Event' object is not created.
	Event(uint64_t ID);

	// Has the event been handled?
	bool consumed = false;
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

	// The postion of the mouse event.
	glm::vec2 mousePosition = { 0.f, 0.f };

protected:

	// Constructor that sets the mouse position.
	// This is a protected type to ensure that a
	// 'MouseEvent' object is not created.
	MouseEvent(glm::vec2& positionPixels, uint64_t ID);
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

	// How much the mouse wheel scrolled.
	float yOffset = 0;
};

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

class KeyEvent : public Event
{

public:

	// Constructor.
	KeyEvent(int key, uint64_t ID, glm::vec2& mousePos);

	// Key associated with the event.
	int key;

	// Position of the mouse when the key event ocurred.
	glm::vec2 mousePosition;

};

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

class WindowEvent : public Event
{

public:

	// Constructor.
	WindowEvent(glm::vec2& windowResize, uint64_t ID, bool isScale = false);

	// For resize events it is the new size, or the scaling.
	// For move events it is the new position.
	glm::vec2 windowData = { 0.f, 0.f };

	// Is the resize value given in scale?
	bool isScale = false;

};

//==============================================================================================================================================//
//  File drop event.																															//
//==============================================================================================================================================//

class FileDropEvent : public Event
{
public:

	// Constructor.
	FileDropEvent(std::vector<std::string>& paths);

	// The path to the dropped files.
	std::unique_ptr<std::vector<std::string>> filePaths;
		
};

//==============================================================================================================================================//
//  Engine Specific Events.																														//
//==============================================================================================================================================//

class LayerEvent : public Event
{

public:

	// Constructor.
	LayerEvent(uint64_t ID);
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//