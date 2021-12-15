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
#include "CoreGL/EngineCoreGL.h"
#include "Resources/ResourceHandler.h"
#include "Graphics/OpenGL/Design2DEngineGL/Design2D_Engine.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
GraphicsHandler::GraphicsHandler(GUIState* guiState, GLFWwindow* glfwWindow) : 
	m_guiState(guiState), m_glfwWindow(glfwWindow) 
{};

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
	if (m_guiState->renderResizeEvent)	{ resizeEvent(); }
	// Check for file drop event.
	if (m_fileDropEvent.eventTrigger)   { fileDropEventHandler(); }
	// Check for save event.
	if (m_saveEvent.eventTrigger)		{ saveEventHandler(); }
	// Check for load event.
	if (m_loadEvent.eventTrigger)		{ loadEventHandler(); }
	// Add window event.
	if (m_addWindow)					{ addWindow(m_newWindowTitle, "Design2D"); m_addWindow = false; }
	// Mouse events.
	if (m_inputEvent.mousePressEvent)   { mousePressEvent(); m_inputEvent.mousePressEvent = false; }
	if (m_inputEvent.mouseScrollEvent)  { mouseScrollEvent(); m_inputEvent.mouseScrollEvent = false; }
	if (m_inputEvent.mouseMoveEvent)	{ mouseMoveEvent(); m_inputEvent.mouseMoveEvent = false; }
	// Key event.
	if (m_inputEvent.keyEvent)			{ keyEvent(); m_inputEvent.keyEvent = false; }
	
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
	// Reset press event.
	m_inputEvent.mousePressEvent = false;

	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		// Find cursos position from ImGui.
		float mousePos[2] = { m_activeWindow->mouseCoords[0] , m_activeWindow->mouseCoords[1] };

		// Left press.
		if (m_inputEvent.mousePressLeftEvent)
		{
			// Call active engine.
			m_activeWindow->engineGL->mousePressLeft(mousePos);
			// Close popup menu.
			m_guiState->popUpMenu = false;
		}

		// Right press.
		if (m_inputEvent.mousePressRightEvent)
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
		if (m_inputEvent.mousePressMiddleEvent)
		{
			// Call active engine.
			m_activeWindow->engineGL->mousePressMiddle(mousePos);
		}
	}

	// Reset the press data.
	m_inputEvent.resetMousePress();
}

void GraphicsHandler::mouseMoveEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		// Find cursos position.
		float mousePos[2] = { m_activeWindow->mouseCoords[0] , 
							  m_activeWindow->mouseCoords[1] };
		m_activeWindow->engineGL->mouseMoveEvent(mousePos, m_inputEvent.mouseMoveButtonStateLeft, 
														   m_inputEvent.mouseMoveButtonStateRight,
														   m_inputEvent.mouseMoveButtonStateMiddle);
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
		m_activeWindow->engineGL->mouseScrollEvent(mousePos, m_inputEvent.mouseScrollY);
	}
}

void GraphicsHandler::keyEvent()
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isActiveWindowValid())
	{
		m_activeWindow->engineGL->keyEvent(m_inputEvent.key, m_inputEvent.keyAction);
	}
}

//=============================================================================================================================================//
//  Window.                                                                   																   //
//=============================================================================================================================================//

bool GraphicsHandler::isWindowValid(std::string windowName) { return m_windowsDictionary.find(windowName) != m_windowsDictionary.end(); }

bool GraphicsHandler::isActiveWindowValid() { return isWindowValid(m_activeWindow); }

bool GraphicsHandler::isWindowValid(std::shared_ptr<RenderWindowGL> renderWindow)
{
	if (renderWindow != NULL) { return m_windowsDictionary.find(renderWindow->windowName) != m_windowsDictionary.end(); }
	else					  { return false; }
}

void GraphicsHandler::resizeEvent()
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
	// Reset boolean.
	m_guiState->renderResizeEvent = false;
}

//=============================================================================================================================================//
//  File events.	                                                                  														   //
//=============================================================================================================================================//

void GraphicsHandler::fileDropEventHandler() 
{
	// Loop through all of the files.
	for (std::string path : m_fileDropEvent.paths)
	{
		// Check if the file is valid.
		if (path.find(".lmct") == std::string::npos &&
			path.find(".yml")  == std::string::npos &&
			path.find(".yaml") == std::string::npos)
		{
			continue;
		}
		// Create a new window.
		addWindow("Generating", "Design2D");
		Design2DEngineGL* engine = reinterpret_cast<Design2DEngineGL*>(m_activeWindow->engineGL.get());
		// Load the circuit data.
		loadFromYAML(*engine, path);
		// Update name.
		engine->m_contextName = engine->m_circuit->m_label;
		auto node = m_windowsDictionary.extract("Generating");
		node.key() = engine->m_contextName;
		m_windowsDictionary.insert(std::move(node));
		m_activeWindow->windowName = engine->m_circuit->m_label;
		m_activeWindow->resizeEvent = true;
	}
	m_guiState->design_engine = reinterpret_cast<Design2DEngineGL*>(m_activeWindow->engineGL.get());
	// Reset event.
	m_fileDropEvent.eventTrigger = false;
	m_fileDropEvent.totalFiles = 0;
	m_fileDropEvent.paths.clear();
	m_fileDropEvent.paths.shrink_to_fit();
	// Set resize.
	m_guiState->renderResizeEvent = true;
}

void GraphicsHandler::saveEventHandler() 
{
	// Check if operation did not fail.
	if (m_loadEvent.path != "OPERATION_CANCELLED" && m_loadEvent.path != "FOLDER_EMPTY")
	{

		// Find engine.
		Design2DEngineGL* saveEngine = reinterpret_cast<Design2DEngineGL*>(m_windowsDictionary[m_saveEvent.saveEngine]->engineGL.get());

		// Check if file is added to the save event.
		std::string savePath = m_saveEvent.path;
		if (savePath.find(".lmct") != std::string::npos ||
			savePath.find(".yml") != std::string::npos ||
			savePath.find(".yaml") != std::string::npos)
		{
			// Move the file onto a new string.
			std::string file;
			while (savePath.back() != '\\')
			{
				file.push_back(savePath.back());
				savePath.pop_back();
			}
			std::reverse(file.begin(), file.end());
			saveToYAML(saveEngine->m_circuit, savePath, file);
		}
		else
		{
			saveToYAML(saveEngine->m_circuit, m_saveEvent.path);
		}
	}
	m_saveEvent.saveEngine = "";
	m_saveEvent.eventTrigger = false;
	m_saveEvent.path = "";

}

void GraphicsHandler::loadEventHandler() 
{
	// Check if operation did not fail.
	if (m_loadEvent.path != "OPERATION_CANCELLED" && m_loadEvent.path != "FOLDER_EMPTY") 
	{ 
		// Load file.
		addWindow("Generating", "Design2D");
		Design2DEngineGL* activeEngine = reinterpret_cast<Design2DEngineGL*>(m_activeWindow->engineGL.get());
		loadFromYAML(*activeEngine, m_loadEvent.path);
		// Update name.
		activeEngine->m_contextName = activeEngine->m_circuit->m_label;
		auto node = m_windowsDictionary.extract("Generating");
		node.key() = activeEngine->m_contextName;
		m_windowsDictionary.insert(std::move(node));
		m_activeWindow->windowName = activeEngine->m_circuit->m_label;
		m_activeWindow->resizeEvent = true;
	}
	// Reset event.
	m_guiState->renderResizeEvent = true;
	m_loadEvent.reset();
}
	
//=============================================================================================================================================//
//  EOF.																																	                                                                     //
//=============================================================================================================================================//