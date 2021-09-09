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
GraphicsHandler::GraphicsHandler(GLFWwindow* window, stateMachineGraphics* states)
	: m_window(window), m_states(states)
{
	// Create engines.
	m_drawingEngine = new BaseEngineGL(m_window);

	//---------------------------------------------------------------------------------------
	// Test code.
	//---------------------------------------------------------------------------------------

	for (int i = 0; i <= 3; i++) 
	{
		for (int k = 0; k <= 3; k++) 
		{
			// Draw filled triangle example.
			float ftPos1[2] = { -1.0f + i, -1.0f + k };
			float ftPos2[2] = { -1.0f + i, -0.5 + k };
			float ftPos3[2] = { -1.5f + i, -1.0f + k };
			float ftColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
			m_drawingEngine->drawTriangleFilled(ftPos1, ftPos2, ftPos3, ftColor);

			// Draw clear quad.
			float cqCoords[2] = { 0.0f + i, 0.0f + k };
			float cqColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			m_drawingEngine->drawQuadClear(cqCoords, 2, 2, cqColor);

			// Draw filled quad.
			float fqCoords[2] = { -1.0f + i, 1.0f + k };
			float fqColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
			m_drawingEngine->drawQuadFilled(fqCoords, 0.25, 0.3, fqColor);

			// Draw filed ciricle.
			float coords1[2] = { 0.0f + i, 0.0f + k };
			float color[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
			m_drawingEngine->drawCircleFilled(coords1, 0.2, color);
			// Draw clear ciricle.
			float coords2[2] = { i, -0.75f + k };
			m_drawingEngine->drawCircleClear(coords2, 0.2, color);

			// Draw clear triangle example.
			float ctPos1[2] = { 1.0f + i, -1.0f + k };
			float ctPos2[2] = { 1.5f + i, -1.0f + k };
			float ctPos3[2] = { 1.0f + i, -0.5f + k };
			float ctColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
			m_drawingEngine->drawTriangleClear(ctPos1, ctPos2, ctPos3, ctColor);
				
			// Render textures
			TexturedVertexData v1(1.25f+i, 1.25f+k, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			TexturedVertexData v2(1.25f+i, 0.75f+k, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
			TexturedVertexData v3(0.75f+i, 0.75f+k, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			TexturedVertexData v4(0.75f+i, 1.25f+k, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			std::vector<TexturedVertexData> verticesTex = { v1, v2, v3, v3, v4, v1 };
			m_drawingEngine->m_texTrianglesVAO->writeData(verticesTex);
		}
	}

	//---------------------------------------------------------------------------------------

	//DesignEngineGL designEngine(this->window);
	//this->designEngine = designEngine;

	// Set the default active engine.  (Should be set to animation when one is available.)
	m_activeEngine = Engines::BASE_ENGINE;
};

// Destructor.
GraphicsHandler::~GraphicsHandler() 
{
	// Delete engines.
	delete m_drawingEngine;
};

//----------------------------------------------------------------------------------------------------------------------
//  Functions.
//----------------------------------------------------------------------------------------------------------------------

// [LOOP FUNCTION] Function that handles which engine should be active and is placed into the OpenGL loop.
void GraphicsHandler::renderGraphics()
{

	//  Run engine that has been set as active.
	if (m_activeEngine == Engines::BASE_ENGINE)
	{
		m_drawingEngine->renderLoop();
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
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		// Find cursos position.
		double mousePos[2];
		glfwGetCursorPos(window, &mousePos[0], &mousePos[1]);

		if (m_activeEngine == Engines::BASE_ENGINE)
		{
			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_drawingEngine->mousePressLeft(mousePos);
			}

			// Check if left press.
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			{
				// Call active engine.
				m_drawingEngine->mousePressRight(mousePos);
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
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		// Find cursos position.
		double mousePos[2] = { xpos, ypos };
		// Check button state.
		int buttonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Call active engine.
		if (m_activeEngine == Engines::BASE_ENGINE) 
		{ 
			m_drawingEngine->mouseMoveEvent(mousePos, buttonState); 
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
	if (!ImGui::GetIO().WantCaptureMouse) 
	{
		// Find cursos position.
		double mousePos[2];
		glfwGetCursorPos(window, &mousePos[0], &mousePos[1]);

		// Call current active engine zoom function.
		if (m_activeEngine == Engines::BASE_ENGINE) 
		{
			m_drawingEngine->mouseScrollEvent(mousePos, yoffset);
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

void GraphicsHandler::resizeEvent(GLFWwindow* window, int width, int height)
{
	// Call resize on active engine.
	if (m_activeEngine == Engines::BASE_ENGINE)
	{
		m_drawingEngine->resizeEvent(width, height);
	}
	else 
	{
		std::cout << "[OPENGL][ERROR][ENGINES] Active engine not found during resize event.\n";
	}
	
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------