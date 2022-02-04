//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events.h"
#include "GLFW/glfw3.h"
#include "ImGUI/Implementations/imgui_impl_glfw.h"

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

glm::vec2& MouseEvent::getPositionGLFW()
{
	return m_mousePositionGLFW;
}

MouseEvent::MouseEvent(glm::vec2& positionPixels, uint64_t ID)
	: Event(ID), m_mousePositionGLFW(positionPixels)
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
	: MouseEvent(mousePositionPixels, ID | EventType::MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(glm::vec2 mousePositionPixels, float yOffset, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType::MouseScroll), m_yOffset(yOffset) 
{}

float MouseScrollEvent::getYOffset() 
{
	return m_yOffset;
}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t ID)
	: Event(ID), m_key(key) 
{}

int KeyEvent::getKey()
{
	return m_key;
}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowResizeEvent::WindowResizeEvent(glm::vec2& windowSize)
	: Event(EventType::WindowResize), m_windowSize(windowSize)
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
//  EOF.																																		//
//==============================================================================================================================================//
