#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "glm/glm.hpp"
#include <filesystem>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;

//==============================================================================================================================================//
//  Event types.																																//
//==============================================================================================================================================//

enum EventType : uint64_t
{
	// Layer specific identifiers.
	EventType_Error				=	0,
	EventType_Application		=	1 << 0,

	// Mouse events.
	EventType_MousePress		=	1 << 1,
	EventType_MouseRelease		=	1 << 2,
	EventType_MouseDoublePress	=	1 << 3,
	EventType_MouseScroll		=	1 << 4,
	EventType_MouseMove			=	1 << 5,
	EventType_MouseDrag			=	1 << 6,
	// Mouse button states.
	EventType_MouseButtonLeft	=	1 << 7,
	EventType_MouseButtonRight	=	1 << 8,
	EventType_MouseButtonMiddle	=	1 << 9,

	// Key events.
	EventType_KeyPress			=	1 << 10,
	EventType_KeyRelease		=	1 << 11,
	EventType_KeyRepeat			=	1 << 12,
	// Key states.
	EventType_LeftCtrl			=	1 << 13,
	EventType_RightCtrl			=	1 << 14,
	EventType_LeftShift			=	1 << 15,
	EventType_RightShift		=	1 << 16,
	EventType_LeftAlt			=	1 << 17,
	EventType_RightAlt			=	1 << 18,

	// Window events.
	EventType_WindowResize		=	1 << 19,
	EventType_WindowMove		=	1 << 20,
	EventType_WindowClose		=	1 << 21,

	// File events.
	EventType_FileDrop			=	1 << 22,
	EventType_FileSave			=	1 << 23,
	EventType_FileLoad			=	1 << 24,

	// Notify events.
	EventType_Notify			=	1 << 25,
	EventType_Focus				=	1 << 26,
	EventType_Defocus			=	1 << 27,
	EventType_Hover				=	1 << 28,
	EventType_Dehover			=	1 << 29,
	EventType_MouseDragStart	=	1 << 30,
	EventType_MouseDragStop		=	1 << 31
};

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

class Event
{

public:

	// Consumes the event, preventing it from being used by other layers.
	inline void consume() 
	{
		consumed = true;
	}
	// Checks if the event has been consumed and if it should not be handled.
	inline bool isConsumed() 
	{
		return consumed;
	}
	// Destructor (for polymorphic type).
	virtual ~Event() = default;
	// Check if the event is of a certain type.
	// Does NOT check if it ONLY belongs to the specific ID.
	inline bool isType(uint64_t compareID) 
	{
		return ( compareID & ID ) == compareID;
	}
	// Check if the event is not of a certain type.
	// Does NOT check if it ONLY belongs to the specific ID.
	inline bool isNotType(uint64_t compareID) 
	{
		return (compareID & ID) != compareID;
	}

	// ID describing the event.
	uint64_t ID;

protected:
	 
	// Constructor that sets the ID of the event.
	// This is a protected type to ensure that an 
	// 'Event' object is not created.
	inline Event(uint64_t ID) 
		: ID(ID)
	{}

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

	inline MouseEvent(const glm::vec2& positionPixels, uint64_t ID) 
		: Event(ID), mousePosition(positionPixels)
	{}
};

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

class MouseButtonEvent : public MouseEvent 
{

public:

	inline MouseButtonEvent(const glm::vec2& mousePositionPixels, uint64_t ID) 
		: MouseEvent(mousePositionPixels, ID)
	{}
};

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

class MouseMoveEvent : public MouseEvent
{

public:

	inline MouseMoveEvent(const glm::vec2& mousePositionPixels, uint64_t ID) 
		: MouseEvent(mousePositionPixels, ID | EventType_MouseMove)
	{}
};

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

class MouseScrollEvent : public MouseEvent
{

public:

	inline MouseScrollEvent(const glm::vec2& mousePositionPixels, float yOffset, float xOffset, uint64_t ID) 
		: MouseEvent(mousePositionPixels, ID | EventType_MouseScroll), yOffset(yOffset), xOffset(xOffset)
	{}

	// How much the mouse wheel scrolled.
	float yOffset = 0;
	float xOffset = 0;
};

// --------------------- //
//  M O U S E   D R A G  //
// --------------------- //

class MouseDragEvent : public MouseEvent
{
public:

	inline MouseDragEvent(const glm::vec2& init, const glm::vec2& current, uint64_t ID)
		: MouseEvent(current, ID | EventType_MouseDrag), initialPosition(init)
	{}

	glm::vec2 initialPosition;
};

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

class KeyEvent : public Event
{

public:

	inline KeyEvent(int key, uint64_t ID, const glm::vec2& mousePos) 
		: Event(ID), key(key), mousePosition(mousePos)
	{}

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

	inline WindowEvent(const glm::vec2& windowResize, uint64_t ID, bool isScale = false) 
		: Event(ID), windowData(windowResize), isScale(isScale)
	{}

	// For resize events it is the new size, or the scaling.
	// For move events it is the new position.
	glm::vec2 windowData = { 0.f, 0.f };

	// Is the resize value given in scale?
	bool isScale = false;
};

//==============================================================================================================================================//
//  File events.																																//
//==============================================================================================================================================//

// --------- //
//  F I L E  //
// --------- //

class FileEvent : public Event
{
public:

	// The files related to the event.
	std::vector<std::filesystem::path> fileData;

protected:

	inline FileEvent(uint64_t eventID, const std::vector<std::filesystem::path>& files) 
		: Event(eventID | EventType_Application), fileData(files)
	{}

	inline FileEvent(uint64_t eventID, const std::filesystem::path& file)
		: Event(eventID | EventType_Application)
	{
		fileData.emplace_back(file);
	}
};

// ------------------- //
//  F I L E   L O A D  //
// ------------------- //

class FileLoadEvent : public FileEvent
{

public: 

	inline FileLoadEvent(const std::vector<std::filesystem::path>& files) 
		: FileEvent(EventType_FileLoad, files)
	{}

	inline FileLoadEvent(const std::filesystem::path& file) 
		: FileEvent(EventType_FileLoad, file)
	{}
};

// ------------------- //
//  F I L E   S A V E  //
// ------------------- //

class FileSaveEvent : public FileEvent
{

public:

	inline FileSaveEvent(const std::vector<std::filesystem::path>& files, EngineCore* engine) 
		: FileEvent(EventType_FileSave, files), engine(engine)
	{}

	inline FileSaveEvent(const std::filesystem::path& file, EngineCore* engine) 
		: FileEvent(EventType_FileSave, file), engine(engine)
	{}

	template<class EngineType>
	inline EngineType* getEngine() 
	{
		return dynamic_cast<EngineType*>(engine);
	}

private:

	// The engine that is to be saved.
	EngineCore* engine = nullptr;
};

// ------------------ //
//  F I L E  D R O P  //
// ------------------ //

class FileDropEvent : public FileEvent
{

public:

	inline FileDropEvent(const std::vector<std::filesystem::path>& files) 
		: FileEvent(EventType_FileDrop, files)
	{}

	inline FileDropEvent(const std::filesystem::path& file) 
		: FileEvent(EventType_FileDrop, file)
	{}
};

//==============================================================================================================================================//
//  Layer Events.																																//
//==============================================================================================================================================//

class NotifyEvent : public Event
{

public:

	inline NotifyEvent(uint64_t ID) 
		: Event(ID | EventType_Notify)
	{}

	inline NotifyEvent(uint64_t ID, const std::string& msg) 
		: Event(ID | EventType_Notify), msg(msg)	
	{}

	// Notify message.
	std::string msg = "";
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//