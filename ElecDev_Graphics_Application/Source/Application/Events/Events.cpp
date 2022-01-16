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
	: Event(eventID), m_mousePositionGLFW(positionPixels)
{}

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

MouseButtonEvent::MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID)
{}

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

MouseMoveEvent::MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID | EventType::MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t eventID)
	: MouseEvent(mousePositionPixels, eventID | EventType::MouseScroll), m_yOffset(yOffset) 
{}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t eventID)
	: Event(eventID), m_key(key) 
{}

int KeyEvent::getKey()
{
	return m_key;
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowResizeEvent::WindowResizeEvent(glm::vec2& windowSize, uint64_t eventID)
	: Event(eventID), m_windowSize(windowSize)
{}

glm::vec2& WindowResizeEvent::getWindowSize()
{
	return m_windowSize;
}

//==============================================================================================================================================//
//  Serialisation events.																														//
//==============================================================================================================================================//

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

FileDropEvent::FileDropEvent(std::vector<std::string>& paths)
	: Event(EventType::FileDrop)
{
	filePaths = std::make_unique<std::vector<std::string>>(std::move(paths));
}

// Get the dropped file.
std::vector<std::string>& FileDropEvent::getPaths()
{
	return *filePaths.get();
}

//==============================================================================================================================================//
//  Event Log.																																	//
//==============================================================================================================================================//

EventLog::EventLog()
{
	// To prevent unnecesary copying of data.
	keyPressEvents.reserve(10);
	keyReleaseEvents.reserve(10);
}

// Here the mouse move event has to be checked first, becuase if a button is
// pressed during the move event it is logged in the ID.  If it is then first 
// checked for being a button event it will miss the fact that it should be 
// a move event.
void EventLog::log(Event& event)
{
	// Get the event ID.
	uint64_t eventID = event.getID();

	// Mouse events.
	if		( eventID == EventType::MouseMove	)	{ mouseMoveEvent	= std::make_unique<MouseMoveEvent>(std::move(dynamic_cast<MouseMoveEvent&>(event)));		}
	else if ( eventID == EventType::MouseScroll	)	{ mouseScrollEvent	= std::make_unique<MouseScrollEvent>(std::move(dynamic_cast<MouseScrollEvent&>(event)));	}
	else if ( eventID == EventType::MousePress	)	{ mousePressEvent	= std::make_unique<MouseButtonEvent>(std::move(dynamic_cast<MouseButtonEvent&>(event)));	}
	else if ( eventID == EventType::MouseRelease)	{ mouseReleaseEvent = std::make_unique<MouseButtonEvent>(std::move(dynamic_cast<MouseButtonEvent&>(event)));	}

	// Key events.
	else if ( eventID == EventType::KeyPress	)	{ keyPressEvents.emplace_back(std::make_unique<KeyEvent>(std::move(dynamic_cast<KeyEvent&>(event))));			}
	else if ( eventID == EventType::KeyRelease	)	{ keyReleaseEvents.emplace_back(std::make_unique<KeyEvent>(std::move(dynamic_cast<KeyEvent&>(event))));			}

	// Window events.
	else if ( eventID == EventType::WindowResize)	{ windowResizeEvent	= std::make_unique<WindowResizeEvent>(std::move(dynamic_cast<WindowResizeEvent&>(event)));	}

	// Serialisation events.
	else if (eventID == EventType::FileDrop		)	{ fileDropEvent		= std::make_unique<FileDropEvent>(std::move(dynamic_cast<FileDropEvent&>(event)));			}
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
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
