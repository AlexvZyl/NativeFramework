/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will contain both ImGUI calls and pure OpenGL calls can remain clean.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "GraphicsHandler.h"
#include <iostream>
#include "Utilities/Serialisation/Serialiser.h"
#include "Design2DEngineGL/Peripherals/Circuit.h"
#include "GLFW/glfw3.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
GraphicsHandler::GraphicsHandler(GUIState* guiState, GLFWwindow* glfwWindow) : 
	m_guiState(guiState), m_glfwWindow(glfwWindow) {};

// Destructor.
GraphicsHandler::~GraphicsHandler(){};

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderLoop()
{
	// ------------- //
	//  E V E N T S  //
	// ------------- //

	// Resize event.
	if (m_guiState->renderResizeEvent) { resizeEvent((int)m_guiState->renderWindowSize[0], (int)m_guiState->renderWindowSize[1]); }
	// Mouse events.
	if (inputEvent.mousePressEvent)    { mousePressEvent(); inputEvent.mousePressEvent = false; }
	if (inputEvent.mouseScrollEvent)   { mouseScrollEvent(); inputEvent.mouseScrollEvent = false; }
	if (inputEvent.mouseMoveEvent)	   { mouseMoveEvent(); inputEvent.mouseMoveEvent = false; }
	// Key event.
	if (inputEvent.keyEvent)		   { keyEvent(); inputEvent.keyEvent = false; }
	// Add window event.
	if (m_addWindow)			       { addWindow(m_newWindowTitle, "Design2D"); m_addWindow = false; }
	// Check for file drop event.
	if (fileDropEvent.eventTrigger)    { fileDropEventHandler(); }
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Check if there are windows to render.
	if (m_windowsDictionary.size())
	{
		// Render each OpenGL context.
		for (auto& [name, window] : m_windowsDictionary) { window->engineGL->renderLoop(); }
	}
};

//=============================================================================================================================================//
//  User Input.																																   //
//=============================================================================================================================================//

void GraphicsHandler::mousePressEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		// Find cursos position from ImGui.
		float mousePos[2] = { m_activeWindow->mouseCoords[0] , m_activeWindow->mouseCoords[1] };

		// Left press.
		if (inputEvent.mousePressButton == GLFW_MOUSE_BUTTON_LEFT && 
			inputEvent.mousePressAction == GLFW_PRESS)
		{
			// Call active engine.
			m_activeWindow->engineGL->mousePressLeft(mousePos);
			// Close popup menu.
			m_guiState->popUpMenu = false;
		}

		// Right press.
		else if (inputEvent.mousePressButton == GLFW_MOUSE_BUTTON_RIGHT && 
				 inputEvent.mousePressAction == GLFW_PRESS)
		{
			// Call active engine.
			m_activeWindow->engineGL->mousePressRight(mousePos);
			// Open menu if in scene.
			if (isWindowValid(m_activeWindow)) 
			{
				m_guiState->popUpMenu = true;
				m_guiState->popUpPosition = {m_guiState->imguiGlobalMouseCoords};
			}
		}

		// Middle press.
		else if (inputEvent.mousePressButton == GLFW_MOUSE_BUTTON_MIDDLE &&
				 inputEvent.mousePressAction == GLFW_PRESS)
		{
			// Call active engine.
			m_activeWindow->engineGL->mousePressMiddle(mousePos);
		}
	}
}

void GraphicsHandler::mouseMoveEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		// Find cursos position.
		float mousePos[2] = { m_activeWindow->mouseCoords[0] , 
							  m_activeWindow->mouseCoords[1] };
		m_activeWindow->engineGL->mouseMoveEvent(mousePos, inputEvent.mouseMoveButtonStateLeft, 
														   inputEvent.mouseMoveButtonStateRight,
														   inputEvent.mouseMoveButtonStateMiddle);
		// Update the GUI state mouse coords.
		m_guiState->renderWindowMouseCoordinate[0] = mousePos[0];
		m_guiState->renderWindowMouseCoordinate[1] = mousePos[1];
	}
}

void GraphicsHandler::mouseScrollEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		// Find cursor position.
		float mousePos[2] = { m_activeWindow->mouseCoords[0] , m_activeWindow->mouseCoords[1] };
		m_activeWindow->engineGL->mouseScrollEvent(mousePos, inputEvent.mouseScrollY);
	}
}

void GraphicsHandler::keyEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		m_activeWindow->engineGL->keyEvent(inputEvent.key, inputEvent.keyAction);
	}
}

//=============================================================================================================================================//
//  Window.                                                                   																   //
//=============================================================================================================================================//

bool GraphicsHandler::isWindowValid(std::shared_ptr<RenderWindowGL> renderWindow)
{
	if (renderWindow != NULL) { return m_windowsDictionary.find(renderWindow->windowName) != m_windowsDictionary.end(); }
	else { return false; }
}

bool GraphicsHandler::isWindowValid(std::string windowName) { return m_windowsDictionary.find(windowName) != m_windowsDictionary.end(); }

bool GraphicsHandler::isActiveWindowValid() { return isWindowValid(m_activeWindow); }

void GraphicsHandler::resizeEvent(int width, int height)
{
	// Check if the dict is not empty.	
	if (m_windowsDictionary.size())
	{
		// Run resize event for each engine.
		for (auto const& [name, window] : m_windowsDictionary)
		{
			if (window->resizeEvent)
			{
				window->engineGL->resizeEvent(window->viewportDimentions[0], window->viewportDimentions[1]);
				window->resizeEvent = false;
			}
		}
	}
}

//=============================================================================================================================================//
//  File Drop Event.                                                                   														   //
//=============================================================================================================================================//

void GraphicsHandler::fileDropEventHandler() 
{
	// Loop through all of the files.
	for (std::string path : fileDropEvent.paths)
	{
		// Create a new window.
		addWindow("Generating", "Design2D");
		Design2DEngineGL* engine = reinterpret_cast<Design2DEngineGL*>(m_activeWindow->engineGL.get());
		// Load the circuit data.
		loadCircuitFromYAML(path, engine);
		// Update name.
		engine->m_contextName = engine->m_circuit->m_label;
		auto node = m_windowsDictionary.extract("Generating");
		node.key() = engine->m_contextName;
		m_windowsDictionary.insert(std::move(node));
		m_activeWindow->windowName = engine->m_circuit->m_label;
	}
	// Reset event.
	fileDropEvent.eventTrigger = false;
	fileDropEvent.totalFiles = 0;
	fileDropEvent.paths.clear();
	fileDropEvent.paths.shrink_to_fit();
}
	
//=============================================================================================================================================//
//  EOF.																																	                                                                     //
//=============================================================================================================================================//