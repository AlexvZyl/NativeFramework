#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include <stdint.h>
#include <memory>
#include "GLFW/glfw3.h"
#include "External/GLM/glm.hpp"
#include "ImGUI/Implementations/imgui_impl_glfw.h"

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
	FolderDrop			=	1 << 11

};

// Check if an ID contains a specific type.
bool operator==(uint64_t id, EventType eventType) 
{
	return id & eventType;
}

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
	// We do not want the user to be able to create 'Event' objects.
	friend class MouseEvent;
	friend class KeyEvent;
	friend class WindowResizeEvent;
	friend class FileDropEvent;
	friend class FolderDropEvent;

	// Constructor that sets the ID of the event.
	Event(uint64_t eventID) 
	{ 
		m_eventID = eventID; 
	}

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
	glm::vec2& getPositionPixels() 
	{
		return m_mousePositionGLFW;
	}

private:

	// Classes that can use 'MouseEvent'.
	// We do not want the user to be able to create 'MouseEvent' objects.
	friend class MouseButtonEvent;
	friend class MouseMoveEvent;
	friend class MouseScrollEvent;

	// Constructor that sets the mouse position.
	MouseEvent(glm::vec2& positionPixels, uint64_t eventID) 
		: Event(eventID) 
	{
		m_mousePositionGLFW = positionPixels;
	};

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

// This might rather be handled differently, since ImGUI keeps track
// of window sizes.
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
//  Serialisation events.																														//
//==============================================================================================================================================//

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

class FileDropEvent : public Event
{

		
};

// ----------------------- //
//  F O L D E R   D R O P  //
// ----------------------- //

class FolderDropEvent : public Event
{


};


//==============================================================================================================================================//
//  Event states.																																//
//==============================================================================================================================================//

struct EventLog
{
	// Mouse events.
	std::unique_ptr<MouseButtonEvent> mouseButtonEvent = nullptr;
	std::unique_ptr<MouseMoveEvent>   mouseMoveEvent = nullptr;
	std::unique_ptr<MouseScrollEvent> mouseScrollEvent = nullptr;
	
	// Key events.
	std::unique_ptr<KeyEvent> keyEvent = nullptr;

	// Window events.
	std::unique_ptr<WindowResizeEvent> windowResizeEvent = nullptr;

	// Serialisation events.
	std::unique_ptr<FileDropEvent> fileDropEvent = nullptr;
	std::unique_ptr<FolderDropEvent> folderDropEvent = nullptr;

	// Add event to the log.
	void onEvent(Event& event)
	{
		// Mouse events.
		if		(event.getID() == EventType::MouseMove)		{ mouseMoveEvent	= std::make_unique<MouseMoveEvent>(dynamic_cast<MouseMoveEvent&>(event)); }
		else if (event.getID() == EventType::MouseScroll)	{ mouseScrollEvent	= std::make_unique<MouseScrollEvent>(dynamic_cast<MouseScrollEvent&>(event)); }
		else if	(event.getID() == EventType::MousePress)	{ mouseButtonEvent	= std::make_unique<MouseButtonEvent>(dynamic_cast<MouseButtonEvent&>(event)); }
		else if	(event.getID() == EventType::MouseRelease)	{ mouseButtonEvent  = std::make_unique<MouseButtonEvent>(dynamic_cast<MouseButtonEvent&>(event)); }

		// Key events.
		else if (event.getID() == EventType::KeyPress)		{ keyEvent			= std::make_unique<KeyEvent>(dynamic_cast<KeyEvent&>(event)); }
		else if (event.getID() == EventType::KeyRelease)	{ keyEvent			= std::make_unique<KeyEvent>(dynamic_cast<KeyEvent&>(event)); }

		// Window events.
		else if (event.getID() == EventType::WindowResize)	{ windowResizeEvent = std::make_unique<WindowResizeEvent>(dynamic_cast<WindowResizeEvent&>(event)); }

		// Serialisation events.
		else if (event.getID() == EventType::FileDrop)		{ fileDropEvent		= std::make_unique<FileDropEvent>(dynamic_cast<FileDropEvent&>(event)); }
		else if (event.getID() == EventType::FolderDrop)	{ folderDropEvent	= std::make_unique<FolderDropEvent>(dynamic_cast<FolderDropEvent&>(event)); }
	}
};

// State used throughout the application.
// Has to be global so that GLFW can use it for callbacks.
EventLog eventLog;

//==============================================================================================================================================//
//  GLFW.																																		//
//==============================================================================================================================================//

void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	// Create the event ID.
	uint64_t eventID = 0;

	// Event type.
	if		(action == GLFW_PRESS)				 { eventID |= EventType::MousePress; }
	else if (action == GLFW_RELEASE)			 { eventID |= EventType::MouseRelease; }

	// Button states.
	if		(button == GLFW_MOUSE_BUTTON_LEFT)   { eventID |= EventType::MouseButtonLeft; }
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)  { eventID |= EventType::MouseButtonRight; }
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) { eventID |= EventType::MouseButtonMiddle; }

	// Get the cursos position.
	glm::vec2 cursorPos;
	glfwGetCursorPos(window, (double*) &cursorPos.x, (double*) & cursorPos.y);

	// Log event.
	MouseButtonEvent event(cursorPos, eventID);
	eventLog.onEvent(event);

	// ImGui mouse callback.
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
	// Event ID.
	uint64_t eventID = EventType::MouseMove;
	
	// Set button states.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType::MouseButtonLeft; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType::MouseButtonRight; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType::MouseButtonMiddle; }

	// Get the cursos position.
	glm::vec2 cursorPos;
	glfwGetCursorPos(window, (double*)&cursorPos.x, (double*)&cursorPos.y);

	// Log event.
	MouseMoveEvent event(cursorPos, eventID);
	eventLog.onEvent(event);
}

void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	// Event ID.
	uint64_t eventID = EventType::MouseScroll;

	// Set button states.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))   { eventID |= EventType::MouseButtonLeft; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))  { eventID |= EventType::MouseButtonRight; }
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) { eventID |= EventType::MouseButtonMiddle; }

	// Get the cursos position.
	glm::vec2 cursorPos;
	glfwGetCursorPos(window, (double*)&cursorPos.x, (double*)&cursorPos.y);

	// Log event.
	MouseScrollEvent event(cursorPos, yoffset, eventID);
	eventLog.onEvent(event);

	// ImGui scroll callback.
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Event ID.
	uint64_t eventID = 0;

	// Event type.
	if	    (action == GLFW_PRESS)   { eventID |= EventType::KeyPress; }
	else if (action == GLFW_RELEASE) { eventID |= EventType::KeyRelease; }

	// Log event.
	KeyEvent event(key, eventID);
	eventLog.onEvent(event);

	// ImGui key callback.
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void fileDropEvent(GLFWwindow* window, int count, const char** paths)
{
	/*graphicsHandler->m_fileDropEvent.eventTrigger = true;
	graphicsHandler->m_fileDropEvent.totalFiles = count;
	for (int i = 0; i < count; i++) { graphicsHandler->m_fileDropEvent.paths.push_back(paths[i]); }*/
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
