/*
All of the CoreEngineGL functions.  Some are general enough to be implemented here.
The rest of the functions are going to be implementd in the respective engines.
If a function is called that is not implemented in the child engine an error is
thrown via functionNotImplementedError().  This also makes it easier to catch errors
and notify the user via the terminal interface.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include <iostream>
#include <memory>
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/CameraGL.h"
#include "Engines/Core/EngineCore.h"
#include "External/GLM/gtc/matrix_transform.hpp"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "External/Misc/ConsoleColor.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
EngineCore::EngineCore(GUIState* guiState, std::string contextName)
	:m_guiState(guiState), m_contextName(contextName)
{
	// Start.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core starting...";

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core done.\n";
}

// Destructor.
EngineCore::~EngineCore() {}

//=============================================================================================================================================//
//  Errors.																																	   //
//=============================================================================================================================================//

void EngineCore::functionNotImplementedError(std::string functionName)
{
	std::cout << red << "\n[OPENGL] [ERROR] : " << white << "'" << functionName << "' is not implemented for the engine type.\n";
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void EngineCore::autoCenter() { functionNotImplementedError(__FUNCTION__); }
void EngineCore::drawDemo(unsigned int loopCount) { functionNotImplementedError(__FUNCTION__); }

void EngineCore::renderLoop() 
{ 
	Renderer::renderScene(m_scene.get()); 
}

unsigned EngineCore::getRenderTexture() 
{ 
	return m_scene->getRenderTexture(); 
}

unsigned EngineCore::getEntityID(glm::vec2& pixelCoords) 
{ 
	return m_scene->getEntityID(pixelCoords); 
}

float EngineCore::deltaTime()
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	return m_deltaTime;
}

//=============================================================================================================================================//
//  User input.																																//
//=============================================================================================================================================//

void EngineCore::mouseScrollEvent(float pixelCoords[2], float yOffset) { functionNotImplementedError(__FUNCTION__); }
void EngineCore::mousePressLeft(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCore::mousePressRight(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCore::mousePressMiddle(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCore::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle) { functionNotImplementedError(__FUNCTION__); }
void EngineCore::keyEvent(int key, int action) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  Viewport.																																   //
//=============================================================================================================================================//

void EngineCore::resizeEvent(float width, float height) 
{ 
	m_scene->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//