//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "imgui/backends/imgui_impl_glfw.h"
#include "Events.h"
#include "Engines/EngineCore/EngineCore.h"
#include "GLFW/glfw3.h"

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
	consumed = true; 
}

bool Event::isConsumed() 
{ 
	return consumed; 
}

Event::Event(uint64_t ID) 
	: ID(ID)
{}

//==============================================================================================================================================//
//  Mouse events.																																//
//==============================================================================================================================================//

// --------------------- //
//  M O U S E   B A S E  //
// --------------------- //

MouseEvent::MouseEvent(const glm::vec2& positionPixels, uint64_t ID)
	: Event(ID), mousePosition(positionPixels)
{}

// ------------------------- //
//  M O U S E   B U T T O N  //
// ------------------------- //

MouseButtonEvent::MouseButtonEvent(const glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID)
{}



// --------------------- //
//  M O U S E   M O V E  //
// --------------------- //

MouseMoveEvent::MouseMoveEvent(const glm::vec2& mousePositionPixels, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseMove)
{}

// ------------------------- //
//  M O U S E   S C R O L L  //
// ------------------------- //

MouseScrollEvent::MouseScrollEvent(const glm::vec2& mousePositionPixels, float yOffset, uint64_t ID)
	: MouseEvent(mousePositionPixels, ID | EventType_MouseScroll), yOffset(yOffset) 
{}

//==============================================================================================================================================//
//  Key Events.																																    //
//==============================================================================================================================================//

KeyEvent::KeyEvent(int key, uint64_t ID, const glm::vec2& mousePos)
	: Event(ID), key(key), mousePosition(mousePos)
{}

//==============================================================================================================================================//
//  Window events.																																//
//==============================================================================================================================================//

WindowEvent::WindowEvent(const glm::vec2& windowResize, uint64_t ID, bool isScale)
	: Event(ID), windowData(windowResize), isScale(isScale)
{}

//==============================================================================================================================================//
//  File events.																																//
//==============================================================================================================================================//

// --------- //
//  F I L E  //
// --------- //

FileEvent::FileEvent(uint64_t eventID, std::vector<std::string>& files)
	: Event(eventID | EventType_Application), fileData(files)
{}

FileEvent::FileEvent(uint64_t eventID, std::string& file)
	: Event(eventID | EventType_Application)
{
	fileData.emplace_back(file);
}

// ------------------- //
//  F I L E   L O A D  //
// ------------------- //

FileLoadEvent::FileLoadEvent(std::vector<std::string>& files)
	: FileEvent(EventType_FileLoad, files)
{}

FileLoadEvent::FileLoadEvent(std::string& file) 
	: FileEvent(EventType_FileLoad, file)
{}

// ------------------- //
//  F I L E   S A V E  //
// ------------------- //

FileSaveEvent::FileSaveEvent(std::vector<std::string>& files, EngineCore* engine)
	: FileEvent(EventType_FileSave, files), engine(engine)
{}

FileSaveEvent::FileSaveEvent(std::string& file, EngineCore* engine)
	: FileEvent(EventType_FileSave, file), engine(engine)
{}

// ------------------- //
//  F I L E   D R O P  //
// ------------------- //

FileDropEvent::FileDropEvent(std::vector<std::string>& files)
	: FileEvent(EventType_FileDrop, files)
{}

FileDropEvent::FileDropEvent(std::string& file)
	: FileEvent(EventType_FileDrop, file)
{}

//==============================================================================================================================================//
//  Layer Events.																																//
//==============================================================================================================================================//

LayerEvent::LayerEvent(uint64_t ID)
	: Event(ID)
{}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//
