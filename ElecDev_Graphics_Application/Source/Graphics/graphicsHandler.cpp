/*
This file will control all of the graphics engines and all of the API's, as well as the unitialization.
This is so that the main loop that will contain both ImGUI calls and pure OpenGL calls can remain clean.
*/

//----------------------------------------------------------------------------------------------------------------------
//  Class include.
//----------------------------------------------------------------------------------------------------------------------

#include "graphicsHandler.h"

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
void GraphicsHandler::mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{	
	//m_windowsDictionary[m_activeWindow]->engineGL.mousepress
}

// Handle mouse move events.
void GraphicsHandler::mouseMoveEvent(GLFWwindow* window, float xpos, float ypos)
{

}

// Handle mouse scroll events.
void GraphicsHandler::mouseScrollEvent(GLFWwindow* window, float xOffset, float yOffset) 
{	

}

//----------------------------------------------------------------------------------------------------------------------
//  Window event handler.
//----------------------------------------------------------------------------------------------------------------------

// Resize event for the ImGui window.
void GraphicsHandler::resizeEvent(int width, int height)
{

}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------