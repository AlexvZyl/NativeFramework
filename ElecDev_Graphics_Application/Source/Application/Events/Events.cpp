//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "ImGUI/Implementations/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include "Events.h"

//==============================================================================================================================================//
//  Event overloaded functions.																													//
//==============================================================================================================================================//

bool operator==(uint64_t id, EventType eventType) 
{ 
	return id & eventType; 
}

bool operator!=(uint64_t id, EventType eventType)
{
	return not (id & eventType);
}

//==============================================================================================================================================//
//  Event Class.																																//
//==============================================================================================================================================//

void Event::consume()
{ 
	m_consumed = true; 
}

bool Event::isConsumed() 
{ 
	return m_consumed; 
}

uint64_t Event::getID() 
{ 
	return m_eventID; 
}

Event::Event(uint64_t ID) 
{ 
	m_eventID = ID; 
}

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

// --------------------- //
//  M O U S E   B A S E  //
// --------------------- //

glm::vec2& MouseEvent::getPosition()
{
	return m_mousePosition;
}

MouseEvent::MouseEvent(glm::vec2& positionPixels, uint64_t ID)
	: Event(ID), m_mousePosition(positionPixels)
{}

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

MouseButtonEvent::MouseButtonEvent(glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID)
{}

// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

MouseMoveEvent::MouseMoveEvent(glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseScroll), m_yOffset(yOffset) 
{}

float MouseScrollEvent::getYOffset() 
{
	return m_yOffset;
}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t ID, glm::vec2& mousePos)
	: Event(ID), m_key(key), m_mousePosition(mousePos)
{}

int KeyEvent::getKey()
{
	return m_key;
}

glm::vec2& KeyEvent::getMousePosition() 
{
	return m_mousePosition;
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowEvent::WindowEvent(glm::vec2& windowResize, uint64_t ID, bool isScale)
	: Event(ID), m_windowData(windowResize), m_isScale(isScale)
{}

glm::vec2& WindowEvent::getWindowData()
{
	return m_windowData;
}

bool WindowEvent::isScale()
{
	return m_isScale;
}

//==============================================================================================================================================//
//  Serialisation events.																														//
//==============================================================================================================================================//

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

FileDropEvent::FileDropEvent(std::vector<std::string>& paths)
	: Event(EventType_FileDrop | EventType_Application)
{
	filePaths = std::make_unique<std::vector<std::string>>(std::move(paths));
}

// Get the dropped file.
std::vector<std::string>& FileDropEvent::getPaths()
{
	return *filePaths.get();
}

//==============================================================================================================================================//
//  Engine Specific Events.																														//
//==============================================================================================================================================//

LayerEvent::LayerEvent(uint64_t ID)
	: Event(ID)
{}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
