#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Events/Events.h"
#include <vector>

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

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the next frame.
	void renderFrame();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Handles all of the events in the event log.
	void dispatchEvents();
	// Handle events specifically for the Application layer.
	void onEvent(Event& event);
	// Log the event in the event log.
	void logEvent(Event& event);

	// ----------------------- //
	//  G L F W   W I N D O W  //
	// ----------------------- //

	// Sets up the GLFW window and OpenGL context.
	static GLFWwindow* glfwInit();
	// Sets up the GLFW callbacks.
	void glfwInitCallbacks();

private:

	// These are to be replavced by the application.
	GraphicsHandler* m_graphicsHandler;
	GUIHandler* m_guiHandler;

	// The window containing the application.
	GLFWwindow* m_window;

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Log containing all of the events.
	std::unique_ptr<EventLog> m_eventLog;
	// Handle window events.
	void onWindowResizeEvent(WindowResizeEvent& event);
	// Handle serialisation events.
	void onFileDropEvent(FileDropEvent& event);

};

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//