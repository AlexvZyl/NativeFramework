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
GraphicsHandler::GraphicsHandler(GLFWwindow* window, stateMachine* states)
	: m_window(window), m_states(states)
{
	//---------------------------------------------------------------------------------------
	// Create engines.
	//---------------------------------------------------------------------------------------

	// Base engine.
	m_mccEngine = new MccEngineGL(m_states);

	//---------------------------------------------------------------------------------------

	// Set the default active engine.  (Should be set to animation when one is available.)
	m_activeEngine = Engines::MCC_ENGINE;
};

// Destructor.
GraphicsHandler::~GraphicsHandler() 
{
	// Delete engines.
	delete m_mccEngine;
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
		resizeEventImGUI(m_states->renderWindowSize.x, m_states->renderWindowSize.y);
	}

	//  Run engine that has been set as active.
	if (m_activeEngine == Engines::MCC_ENGINE)
	{
		m_mccEngine->renderActiveEngine();
	}
	else if (m_activeEngine == Engines::DESIGN_ENGINE)
	{
		// Run designEngine.
	}
	else if (m_activeEngine == Engines::ANIMATION)
	{
		// Run some sort of cool place holder animation.
	}
	else
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Valid engine not found.\n";
	};

};

// Function that closes the engine passed.
void GraphicsHandler::setEngine(std::string engine)
{
	// Close the current active engine.
	closeActiveEngine();

	// Set the new active engine.
	if (m_activeEngine == Engines::BASE_ENGINE)
	{
		// Init.
	}
	else if (m_activeEngine == Engines::DESIGN_ENGINE)
	{
		// Init.
	}
	else if (m_activeEngine == Engines::ANIMATION)
	{
		// Init.
	}
	else
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Valid engine not found.\n";
	};
};

// Function that closes the engine passed.
void GraphicsHandler::closeActiveEngine()
{
	// Close the active engine.
	if (m_activeEngine == Engines::BASE_ENGINE)
	{
		// Close.
	}
	else if (m_activeEngine == Engines::DESIGN_ENGINE)
	{
		// Close.
	}
	else if (m_activeEngine == Engines::ANIMATION)
	{
		// Close animation.
	}
	else
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Error occured closing the active engine.\n";
	};
};

//----------------------------------------------------------------------------------------------------------------------
//  Mouse event handler.
//----------------------------------------------------------------------------------------------------------------------

// Handle mouse press events.
void GraphicsHandler::mousePressEvent(GLFWwindow* window, int button, int action, int mods)
{	
	// Check if not above ImGUI element.
	if (m_states->renderWindowHovered && m_mccEngine->m_mccDictionary.size() != 0)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };

		if (m_activeEngine == Engines::MCC_ENGINE)
		{
			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->engine->mousePressLeft(mousePos);
			}

			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->engine->mousePressRight(mousePos);
			}
		}
		else 
		{
			std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during mouse press event.\n";
		}
	}
}

// Handle mouse move events.
void GraphicsHandler::mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
	// Check if not above ImGUI element.
	if (m_states->renderWindowHovered && m_mccEngine->m_mccDictionary.size() != 0)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };
		// Check button state.
		int buttonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Call active engine.
		if (m_activeEngine == Engines::MCC_ENGINE)
		{ 
			m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->engine->mouseMoveEvent(mousePos, buttonState);
		}
		// Output error if active engine is not found.
		else 
		{ 
			std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during mouse move event.\n"; 
		}
		
	}
}

// Handle mouse scroll events.
void GraphicsHandler::mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset) 
{	
	// Check if not above ImGUI element.
	if (m_states->renderWindowHovered && m_mccEngine->m_mccDictionary.size() != 0)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };

		// Call current active engine zoom function.
		if (m_activeEngine == Engines::MCC_ENGINE)
		{
			m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->engine->mouseScrollEvent(mousePos, yoffset);
		}
		else 
		{
			std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during mouse scroll event.\n";
		}
		
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Window event handler.
//----------------------------------------------------------------------------------------------------------------------

// Resize event for the ImGui window.
void GraphicsHandler::resizeEventImGUI(int width, int height)
{
	// Call resize on active engine.
	if (m_activeEngine == Engines::MCC_ENGINE)
	{
		m_mccEngine->resizeEvent(width, height);
	}
	else 
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during resize event.\n";
	}
	
}



//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------