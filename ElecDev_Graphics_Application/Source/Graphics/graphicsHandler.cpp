/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will contain both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "GraphicsHandler.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructors & setup.
//----------------------------------------------------------------------------------------------------------------------

// With GLFW window.
GraphicsHandler::GraphicsHandler(stateMachine* states)
	: m_states(states)
{
};

// Destructor.
GraphicsHandler::~GraphicsHandler() 
{
};

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// [LOOP FUNCTION] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderGraphics()
{
	// Check for ImGUI viewport resize event.
	if (m_states->renderResizeEvent)
	{
		resizeEvent((int)m_states->renderWindowSize.x, (int)m_states->renderWindowSize.y);
	}

	// Check if there are windows to render.
	if (m_windowsDictionary.size()) 
	{
		// Render each window.
		for (auto window : m_windowsDictionary)
		{
			window.second->engineGL->renderLoop();
		}
	}
};

// Checks if the given name exists in the window dictionary.
bool GraphicsHandler::isWindowValid(std::string windowName)
{
	return m_windowsDictionary.find(windowName) != m_windowsDictionary.end();
}

//----------------------------------------------------------------------------------------------------------------------
//  Mouse event handler.
//----------------------------------------------------------------------------------------------------------------------

// Handle mouse press events.
void GraphicsHandler::mousePressEvent(int button, int action)
{	
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow))
	{
		// Find cursos position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords.x , m_windowsDictionary[m_activeWindow]->mouseCoords.y };

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
	}
}

// Handle mouse move events.
void GraphicsHandler::mouseMoveEvent(int buttonStateLeft, int buttonStateRight)
{
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow))
	{
		// Find cursos position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords.x , m_windowsDictionary[m_activeWindow]->mouseCoords.y };
		m_windowsDictionary[m_activeWindow]->engineGL->mouseMoveEvent(mousePos, buttonStateLeft, buttonStateRight);
	}
}

// Handle mouse scroll events.
void GraphicsHandler::mouseScrollEvent(float yOffset) 
{	
	// Check if window dict is empty or inactive.
	if (m_windowsDictionary.size() && isWindowValid(m_activeWindow)) 
	{
		// Find cursor position.
		float mousePos[2] = { m_windowsDictionary[m_activeWindow]->mouseCoords.x , m_windowsDictionary[m_activeWindow]->mouseCoords.y };
		m_windowsDictionary[m_activeWindow]->engineGL->mouseScrollEvent(mousePos, yOffset);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Window event handler.
//----------------------------------------------------------------------------------------------------------------------

// Resize event for the ImGui window.
void GraphicsHandler::resizeEvent(int width, int height)
{
	// Check if the dict is not empty.	
	if (m_windowsDictionary.size())
	{
		// Run resize event for each engine.
		for (auto engine : m_windowsDictionary)
		{
			if (engine.second->resizeEvent)
			{
				engine.second->engineGL->resizeEvent((int)engine.second->viewportDimentions.x, (int)engine.second->viewportDimentions.y);
				engine.second->resizeEvent = false;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------