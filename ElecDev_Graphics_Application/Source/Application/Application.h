#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events/Events.h"

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class GraphicsHandler;
class GUIHandler;
class EventLog;

struct GLFWwindow;

//==============================================================================================================================================//
//  Application class.																															//
//==============================================================================================================================================//

class Application
{

public:

	// Constructor.
	Application(GLFWwindow* window, GraphicsHandler* graphicsHandler, GUIHandler* guiHandler);

	// Handles all of the events in the event log.
	void handleEvents();

	// Renders the next frame.
	void renderFrame();

private:

	// These are to be replavced by the application.
	GraphicsHandler* m_graphicsHandler;
	GUIHandler* m_guiHandler;

	// The window containing the application.
	GLFWwindow* m_window;

	// Mouse event handling.
	void handleMouseMoveEvent(std::unique_ptr<MouseMoveEvent>& event);
	void handleMouseScrollEvent(std::unique_ptr<MouseScrollEvent>& event);
	void handleMousePressEvent(std::unique_ptr<MouseButtonEvent>& event);
	void handleMouseReleaseEvent(std::unique_ptr<MouseButtonEvent>& event);

	// Key event handling.
	void handleKeyPressEvent(std::unique_ptr<KeyEvent>& event);
	void handleKeyReleaseEvent(std::unique_ptr<KeyEvent>& event);

	// Handle window events.
	void handleWindowResizeEvent(std::unique_ptr<WindowResizeEvent>& event);

	// Handle serialisation events.
	void handleFileDropEvent(std::unique_ptr<FileDropEvent>& event);

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//