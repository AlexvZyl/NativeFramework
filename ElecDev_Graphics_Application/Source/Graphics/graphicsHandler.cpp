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
	m_activeEngine = Engines::BASE_ENGINE;
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
	if (m_activeEngine == Engines::BASE_ENGINE)
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
	if (m_states->renderWindowHovered)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };
		if (m_activeEngine == Engines::BASE_ENGINE)
		{
			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->mousePressLeft(mousePos);
			}

			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->mousePressRight(mousePos);
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
	if (m_states->renderWindowHovered)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };
		// Check button state.
		int buttonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Call active engine.
		if (m_activeEngine == Engines::BASE_ENGINE) 
		{ 
			m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->mouseMoveEvent(mousePos, buttonState);
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
	if (m_states->renderWindowHovered)
	{
		// Find cursos position.
		double mousePos[2] = { m_states->renderWindowMouseCoordinate.x , m_states->renderWindowMouseCoordinate.y };

		// Call current active engine zoom function.
		if (m_activeEngine == Engines::BASE_ENGINE) 
		{
			m_mccEngine->m_mccDictionary[m_mccEngine->m_activeMCC]->mouseScrollEvent(mousePos, yoffset);
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
	if (m_activeEngine == Engines::BASE_ENGINE)
	{
		m_mccEngine->resizeEvent(width, height);
	}
	else 
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during resize event.\n";
	}
	
}

//---------------------------------------------------------------------------------------
// Test code.
//---------------------------------------------------------------------------------------

//for (int i = 0; i <= 1 * 3; i += 3)
//{
//	for (int k = 0; k <= 1 * 3; k += 3)
//	{
//		// Draw filled triangle example.
//		float ftPos1[2] = { -1.0f + i, -1.0f + k };
//		float ftPos2[2] = { -1.0f + i, -0.5 + k };
//		float ftPos3[2] = { -1.5f + i, -1.0f + k };
//		float ftColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
//		m_baseEngine->drawTriangleFilled(ftPos1, ftPos2, ftPos3, ftColor);
//
//		// Draw clear quad.
//		float cqCoords[2] = { 0.0f + i, 0.0f + k };
//		float cqColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
//		m_baseEngine->drawQuadClear(cqCoords, 2, 2, cqColor);
//
//		// Draw filled quad.
//		float fqCoords[2] = { -0.5f + i, 0.5f + k };
//		float fqColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
//		m_baseEngine->drawQuadFilled(fqCoords, 0.25, 0.3, fqColor);
//
//		// Draw filed ciricle.
//		float coords1[2] = { 0.0f + i, 0.0f + k };
//		float color[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
//		m_baseEngine->drawCircleFilled(coords1, 0.2, color);
//		// Draw clear ciricle.
//		float coords2[2] = { i, -0.75f + k };
//		m_baseEngine->drawCircleClear(coords2, 0.2, color);
//
//		// Draw clear triangle example.
//		float ctPos1[2] = { 1.0f + i, -1.0f + k };
//		float ctPos2[2] = { 1.5f + i, -1.0f + k };
//		float ctPos3[2] = { 1.0f + i, -0.5f + k };
//		float ctColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
//		m_baseEngine->drawTriangleClear(ctPos1, ctPos2, ctPos3, ctColor);
//
//		// Test textures.
//		TexturedVertexData v1(1.25f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
//		TexturedVertexData v2(1.25f + i, 0.75f + k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
//		TexturedVertexData v3(0.75f + i, 0.75f + k, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
//		TexturedVertexData v4(0.75f + i, 1.25f + k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
//		std::vector<TexturedVertexData> verticesTex = { v1, v2, v3, v3, v4, v1 };
//		m_baseEngine->m_textureTrianglesVAO->writeData(verticesTex);
//
//		// Test the text rendering.
//		float pos[2] = { 0.5f + i, 0.5f + k };
//		std::string text = "Testing-Font and Different_characters. [!&>*?\\] ";
//		float colorText[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
//		m_baseEngine->drawText(text, pos, colorText, 1);
//	}
//}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------