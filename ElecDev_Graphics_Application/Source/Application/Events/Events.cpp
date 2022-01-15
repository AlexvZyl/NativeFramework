//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include "GLFW/glfw3.h"
#include "ImGUI/Implementations/imgui_impl_glfw.h"

//==============================================================================================================================================//
//  Event overloaded function.																													//
//==============================================================================================================================================//

bool operator==(uint64_t id, EventType eventType)
{
	return id & eventType;
}

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

void Event::handled()
{
	m_handled = true;
}

bool Event::isHandled()
{
	return m_handled;
}

uint64_t Event::getID()
{
	return m_eventID;
}

Event::Event(uint64_t eventID)
{
	m_eventID = eventID;
}

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

// --------------------- //
//  M O U S E   B A S E  //
// --------------------- //

glm::vec2& MouseEvent::getPositionGLFW()
{
	return m_mousePositionGLFW;
}

MouseEvent::MouseEvent(glm::vec2& positionPixels, uint64_t eventID)
	: Event(eventID)
{
	m_mousePositionGLFW = positionPixels;
}

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

MouseButtonEvent::MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID)
{}

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

// Constructor with mouse position.
MouseMoveEvent::MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID | EventType::MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID | EventType::MouseScroll)
{
	m_yOffset = yOffset;
}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t eventID)
	: Event(eventID)
{
	m_key = key;
};

int KeyEvent::getKey()
{
	return m_key;
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowResizeEvent::WindowResizeEvent(glm::vec2& windowSize, uint64_t eventID)
	: Event(eventID)
{
	m_windowSize = windowSize;
}

//==============================================================================================================================================//
//  Serialisation events.																														//
//==============================================================================================================================================//

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

FileDropEvent::FileDropEvent(std::string path)
	: Event(EventType::FileDrop)
{
	filePath = path;
}

// Get the dropped file.
std::string& FileDropEvent::getFile()
{
	return filePath;
}

// ----------------------- //
//  F O L D E R   D R O P  //
// ----------------------- //



//==============================================================================================================================================//
//  Event Log.																																//
//==============================================================================================================================================//

EventLog::EventLog()
{
	// To prevent unnecesary copying of data.
	keyPressEvents.reserve(10);
	keyReleaseEvents.reserve(10);
}

void EventLog::log(Event& event)
{
	// Mouse events.
	if		(event.getID() == EventType::MouseMove)		{ mouseMoveEvent	= std::make_unique<MouseMoveEvent>(dynamic_cast<MouseMoveEvent&>(event)); }
	else if (event.getID() == EventType::MouseScroll)	{ mouseScrollEvent	= std::make_unique<MouseScrollEvent>(dynamic_cast<MouseScrollEvent&>(event)); }
	else if (event.getID() == EventType::MousePress)	{ mousePressEvent	= std::make_unique<MouseButtonEvent>(dynamic_cast<MouseButtonEvent&>(event)); }
	else if (event.getID() == EventType::MouseRelease)	{ mouseReleaseEvent = std::make_unique<MouseButtonEvent>(dynamic_cast<MouseButtonEvent&>(event)); }

	// Key events.
	else if (event.getID() == EventType::KeyPress)		{ keyPressEvents.emplace_back(std::make_unique<KeyEvent>(dynamic_cast<KeyEvent&>(event))); }
	else if (event.getID() == EventType::KeyRelease)	{ keyReleaseEvents.emplace_back(std::make_unique<KeyEvent>(dynamic_cast<KeyEvent&>(event))); }

	// Window events.
	else if (event.getID() == EventType::WindowResize)	{ windowResizeEvent	= std::make_unique<WindowResizeEvent>(dynamic_cast<WindowResizeEvent&>(event)); }

	// Serialisation events.
	else if (event.getID() == EventType::FileDrop)		{ fileDropEvent		= std::make_unique<FileDropEvent>(dynamic_cast<FileDropEvent&>(event)); }
	else if (event.getID() == EventType::FolderDrop)	{ folderDropEvent	= std::make_unique<FolderDropEvent>(dynamic_cast<FolderDropEvent&>(event)); }
}

void EventLog::clear()
{
	// Mouse events.
	mousePressEvent = nullptr;
	mouseReleaseEvent = nullptr;
	mouseMoveEvent = nullptr;
	mouseScrollEvent = nullptr;
	// Key events.
	keyPressEvents.clear();
	keyReleaseEvents.clear();
	// Window events.
	windowResizeEvent = nullptr;
	// Serialisation events.
	fileDropEvent = nullptr;
	folderDropEvent = nullptr;
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//