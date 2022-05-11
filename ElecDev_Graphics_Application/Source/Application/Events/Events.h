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
#include "yaml-cpp/yaml.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class EngineCore;

//==============================================================================================================================================//
//  Event types.																																//
//==============================================================================================================================================//

typedef uint64_t LumenEventID;
enum EventType : LumenEventID
{
	// Layer specific identifiers.
	EventType_Error				=	(LumenEventID)0,
	EventType_Application		=	(LumenEventID)1 << 0,
									
	// Mouse events.				
	EventType_MousePress		=	(LumenEventID)1 << 1,
	EventType_MouseRelease		=	(LumenEventID)1 << 2,
	EventType_MouseDoublePress	=	(LumenEventID)1 << 3,
	EventType_MouseScroll		=	(LumenEventID)1 << 4,
	EventType_MouseMove			=	(LumenEventID)1 << 5,
	EventType_MouseDrag			=	(LumenEventID)1 << 6,
	// Mouse button states.			
	EventType_MouseButtonLeft	=	(LumenEventID)1 << 7,
	EventType_MouseButtonRight	=	(LumenEventID)1 << 8,
	EventType_MouseButtonMiddle	=	(LumenEventID)1 << 9,
									
	// Key events.					
	EventType_KeyPress			=	(LumenEventID)1 << 10,
	EventType_KeyRelease		=	(LumenEventID)1 << 11,
	EventType_KeyRepeat			=	(LumenEventID)1 << 12,
	// Key states.				
	EventType_LeftCtrl			=	(LumenEventID)1 << 13,
	EventType_RightCtrl			=	(LumenEventID)1 << 14,
	EventType_LeftShift			=	(LumenEventID)1 << 15,
	EventType_RightShift		=	(LumenEventID)1 << 16,
	EventType_LeftAlt			=	(LumenEventID)1 << 17,
	EventType_RightAlt			=	(LumenEventID)1 << 18,
									
	// Window events.				
	EventType_WindowResize		=	(LumenEventID)1 << 19,
	EventType_WindowMove		=	(LumenEventID)1 << 20,
	EventType_WindowClose		=	(LumenEventID)1 << 21,
									
	// File events.					
	EventType_FileDrop			=	(LumenEventID)1 << 22,
	EventType_FileSave			=	(LumenEventID)1 << 23,
	EventType_FileLoad			=	(LumenEventID)1 << 24,
	EventType_YamlNodeDrop		=	(LumenEventID)1 << 25,
									
	// Notify events.				
	EventType_Notify			=	(LumenEventID)1 << 26,
	EventType_Focus				=	(LumenEventID)1 << 27,
	EventType_Defocus			=	(LumenEventID)1 << 28,
	EventType_Hover				=	(LumenEventID)1 << 29,
	EventType_Dehover			=	(LumenEventID)1 << 30,
	EventType_MouseDragStart	=	(LumenEventID)1 << 31,
	EventType_MouseDragStop		=	(LumenEventID)1 << 32,
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
	inline bool isConsumed() const
	{
		return consumed;
	}
	// Destructor (for polymorphic type).
	virtual ~Event() = default;

	// Check if the event is of a certain type.
	// Does NOT check if it ONLY belongs to the specific ID.
	inline bool isType(LumenEventID compareID) const 
	{
		return ( compareID & ID ) == compareID;
	}
	inline bool isNotType(LumenEventID compareID) const
	{
		return !isType(compareID);
	}

	// Cast the event to the requested type.
	template<class EventType>
	inline const EventType& cast() const 
	{
		return dynamic_cast<const EventType&>(*this);
	}

	// ID describing the event.
	LumenEventID ID;

protected:
	 
	// Constructor that sets the ID of the event.
	// This is a protected type to ensure that an 
	// 'Event' object is not created.
	inline Event(LumenEventID ID) 
		: ID(ID)
	{}

private:

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

	inline MouseEvent(const glm::vec2& positionPixels, LumenEventID ID) 
		: Event(ID), mousePosition(positionPixels)
	{}
};

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

class MouseButtonEvent : public MouseEvent 
{

public:

	inline MouseButtonEvent(const glm::vec2& mousePositionPixels, LumenEventID ID) 
		: MouseEvent(mousePositionPixels, ID)
	{}
};

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

class MouseMoveEvent : public MouseEvent
{

public:

	inline MouseMoveEvent(const glm::vec2& mousePositionPixels, LumenEventID ID) 
		: MouseEvent(mousePositionPixels, ID | EventType_MouseMove)
	{}
};

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

class MouseScrollEvent : public MouseEvent
{

public:

	inline MouseScrollEvent(const glm::vec2& mousePositionPixels, float yOffset, float xOffset, LumenEventID ID) 
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

	inline MouseDragEvent()
		: MouseEvent({ 0.f, 0.f }, 0 | EventType_MouseDrag), initialPosition({ 0.f, 0.f }), currentFrameDelta({ 0.f, 0.f })
	{}

	inline MouseDragEvent(const glm::vec2& init, const glm::vec2& current, const glm::vec2 delta, LumenEventID ID)
		: MouseEvent(current, ID | EventType_MouseDrag), initialPosition(init), currentFrameDelta(delta)
	{}

	glm::vec2 initialPosition = { 0.f, 0.f };
	glm::vec2 currentFrameDelta = { 0.f, 0.f };
};

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

class KeyEvent : public Event
{

public:

	inline KeyEvent(int key, LumenEventID ID, const glm::vec2& mousePos) 
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

	inline WindowEvent(const glm::vec2& windowResize, LumenEventID ID, bool isScale = false) 
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

	inline FileEvent(LumenEventID eventID, const std::vector<std::filesystem::path>& files) 
		: Event(eventID), fileData(files)
	{}

	inline FileEvent(LumenEventID eventID, const std::filesystem::path& file)
		: Event(eventID)
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

	inline FileLoadEvent(const std::vector<std::filesystem::path>& files, LumenEventID ID = 0) 
		: FileEvent(EventType_FileLoad | ID, files)
	{}

	inline FileLoadEvent(const std::filesystem::path& file, LumenEventID ID = 0) 
		: FileEvent(EventType_FileLoad | ID, file)
	{}
};

// ------------------- //
//  F I L E   S A V E  //
// ------------------- //

class FileSaveEvent : public FileEvent
{

public:

	inline FileSaveEvent(const std::vector<std::filesystem::path>& files, EngineCore* engine, LumenEventID ID = 0)
		: FileEvent(EventType_FileSave | ID, files), engine(engine)
	{}

	inline FileSaveEvent(const std::filesystem::path& file, EngineCore* engine, LumenEventID ID = 0)
		: FileEvent(EventType_FileSave | ID, file), engine(engine)
	{}

	template<class EngineType>
	inline EngineType* getEngine() const 
	{
		return dynamic_cast<EngineType*>(engine);
	}

	inline EngineCore* getEngine() const
	{
		return engine;
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

	inline FileDropEvent(const std::vector<std::filesystem::path>& files, LumenEventID ID = 0) 
		: FileEvent(EventType_FileDrop | ID, files)
	{}

	inline FileDropEvent(const std::filesystem::path& file, LumenEventID ID = 0) 
		: FileEvent(EventType_FileDrop | ID, file)
	{}
};

// ----------------------------- //
//  Y A M L   N O D E   D R O P  //
// ----------------------------- //

class YamlNodeDropEvent : public Event
{

public:

	inline YamlNodeDropEvent(const YAML::Node& node, LumenEventID ID = 0) 
		: Event(ID | EventType_YamlNodeDrop), node(node)
	{}

	const YAML::Node& getNode() const
	{
		return node;
	}

private:
	YAML::Node node;
};

//==============================================================================================================================================//
//  Layer Events.																																//
//==============================================================================================================================================//

class NotifyEvent : public Event
{

public:

	inline NotifyEvent(LumenEventID ID, const std::string& msg = "")
		: Event(ID | EventType_Notify), msg(msg)	
	{}

	// Notify message.
	std::string msg = "";
};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//