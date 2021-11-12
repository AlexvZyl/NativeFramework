/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will contain both ImGUI calls and pure OpenGL calls can remain clean.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include "GraphicsHandler.h"
#include "GLFW/glfw3.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
GraphicsHandler::GraphicsHandler(GUIState* guiState)
	: m_guiState(guiState)
{
};

// Destructor.
GraphicsHandler::~GraphicsHandler() 
{
};

//=============================================================================================================================================//
//  Functions.																																   //
//=============================================================================================================================================//

// [LOOP FUNCTION] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderGraphics()
{
	// Check for ImGUI viewport resize event.
	if (m_guiState->renderResizeEvent)
	{
		resizeEvent((int)m_guiState->renderWindowSize[0], (int)m_guiState->renderWindowSize[1]);
	}

	// Check if there are windows to render.
	if (m_windowsDictionary.size()) 
	{
		// Render each window.
		for (auto& [name, window] : m_windowsDictionary)
		{
			window->engineGL->renderLoop();
		}
	}
};

// Checks if the given name exists in the window dictionary.
bool GraphicsHandler::isWindowValid(std::string windowName)
{
	return m_windowsDictionary.find(windowName) != m_windowsDictionary.end();
}

//=============================================================================================================================================//
//  User Input.																																   //
//=============================================================================================================================================//

void GraphicsHandler::mousePressEvent(int button, int action)
{	
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow))
	{
		// Find cursos position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords[0] , m_windowsDictionary[m_activeWindow]->mouseCoords[1]};

		// Check if left press.
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			// Call active engine.
			m_windowsDictionary[m_activeWindow]->engineGL->mousePressLeft(mousePos);
		}
		// Check if right press.
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			// Call active engine.
			m_windowsDictionary[m_activeWindow]->engineGL->mousePressRight(mousePos);
		}
		// Check if middle press.
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			// Call active engine.
			m_windowsDictionary[m_activeWindow]->engineGL->mousePressMiddle(mousePos);
		}
	}
}

void GraphicsHandler::mouseMoveEvent(int buttonStateLeft, int buttonStateRight, int buttonStateMiddle)
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow))
	{
		// Find cursos position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords[0] , m_windowsDictionary[m_activeWindow]->mouseCoords[1]};
		m_windowsDictionary[m_activeWindow]->engineGL->mouseMoveEvent(mousePos, buttonStateLeft, buttonStateRight, buttonStateMiddle);
		// Update the GUI state mouse coords.
		m_guiState->renderWindowMouseCoordinate[0] = mousePos[0];
		m_guiState->renderWindowMouseCoordinate[1] = mousePos[1];
	}
}

void GraphicsHandler::mouseScrollEvent(float yOffset) 
{	
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow)) 
	{
		// Find cursor position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords[0] , m_windowsDictionary[m_activeWindow]->mouseCoords[1]};
		m_windowsDictionary[m_activeWindow]->engineGL->mouseScrollEvent(mousePos, yOffset);
	}
}

void GraphicsHandler::keyEvent(int key, int action)
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow))
	{
		m_windowsDictionary[m_activeWindow]->engineGL->keyEvent(key, action);
	}
}

//=============================================================================================================================================//
//  Window events.																															   //
//=============================================================================================================================================//

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
//  EOF.																																	   //
//=============================================================================================================================================//