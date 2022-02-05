#pragma once

//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Application/Events/EventLog.h"
#include "Application/Events/Events.h"
#include <vector>

//==============================================================================================================================================//
//  Forward declerations.																														//
//==============================================================================================================================================//

class GraphicsHandler;
class GUIHandler;
class EventLog;
class LayerStack;
class Layer;
class ImFont;

struct GLFWwindow;

//==============================================================================================================================================//
//  Application class.																															//
//==============================================================================================================================================//

class Application
{

public:

	// Constructor.
	Application(GLFWwindow* window);

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Renders the next frame.
	void onRender();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Handles all of the events in the event log.
	void dispatchEvents();
	// Handle events specifically for the Application layer.
	void onEvent(Event& event);
	// Log the event in the event log.
	template <typename EventType>
	void logEvent(Event& event);

	// ----------------------- //
	//  G L F W   W I N D O W  //
	// ----------------------- //

	// Sets up the GLFW window and OpenGL context.
	static GLFWwindow* glfwInitWindow();
	// Sets up the GLFW callbacks.
	void glfwInitCallbacks();

private:

	// The window containing the application.
	GLFWwindow* m_window;

	// ------------- //
	//  L A Y E R S  //
	// ------------- //
	
	// The layers in the application.
	std::unique_ptr<LayerStack> m_layerStack;
	// The layer that has the most recent interaction.
	Layer* m_focusedLayer = nullptr;
	// The layer that the mouse is hovering over.
	Layer* m_hoveredLayer = nullptr;
	// Handle when the hovered layer changes.
	void onHoveredLayerChange(Layer* newLayer);
	// Handle when the focused layer changes.
	void onFocusedLayerChange(Layer* newLayer);
	// Find the layer that is being hovered.
	Layer* findhoveredLayer();

	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Log containing all of the events.
	std::unique_ptr<EventLog> m_eventLog;
	// Handle window events.
	void onWindowResizeEvent(WindowResizeEvent& event);
	// Handle serialisation events.
	void onFileDropEvent(FileDropEvent& event);
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Initialise the frame for rendering.
	void onRenderInit();
	// Cleanup after the frame has been rendered.
	void onRenderCleanup();
	// Set the ImGUI theme.
	void setGuiTheme();
	// The default font used.
	ImFont* m_defaultFont = nullptr;

};


//==============================================================================================================================================//
//  Templates.																																	//
//==============================================================================================================================================//

template <typename EventType>
void Application::logEvent(Event& event)
{
	m_eventLog->log<EventType>(event);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//