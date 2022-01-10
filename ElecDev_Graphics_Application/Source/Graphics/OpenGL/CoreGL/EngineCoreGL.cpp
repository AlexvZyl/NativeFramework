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
#include "Resources/ResourceHandler.h"
#include "CoreGL/EngineCoreGL.h"
#include "CoreGL/Buffers/FrameBufferObjectGL.h"
#include "CoreGL/ShaderGL.h"
#include "CoreGL/Buffers/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/RendererGL.h"
#include "CoreGL/SceneGL.h"
#include "CoreGL/FontLoaderGL.h"
#include "CoreGL/CameraGL.h"
#include "CoreGL/Entities/Entity.h"
#include "CoreGL/Entities/Primitive.h"
#include "External/GLM/gtc/matrix_transform.hpp"
#include "External/GLFW/Includes/GLFW/glfw3.h"
#include "External/Misc/ConsoleColor.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
EngineCoreGL::EngineCoreGL(GUIState* guiState, std::string contextName)
	:m_guiState(guiState), m_contextName(contextName)
{
	// Start.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core starting...";

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core done.\n";
}

// Destructor.
EngineCoreGL::~EngineCoreGL() {}

//=============================================================================================================================================//
//  Errors.																																	   //
//=============================================================================================================================================//

void EngineCoreGL::functionNotImplementedError(std::string functionName)
{
	std::cout << red << "\n[OPENGL] [ERROR] : " << white << "'" << functionName << "' is not implemented for the engine type.\n";
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void EngineCoreGL::autoCenter() { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::drawDemo(unsigned int loopCount) { functionNotImplementedError(__FUNCTION__); }

void EngineCoreGL::renderLoop() { Renderer::renderScene(m_scene.get()); }

unsigned EngineCoreGL::getRenderTexture() { return m_scene->getRenderTexture(); }

unsigned EngineCoreGL::getEntityID(glm::vec2& pixelCoords) { return m_scene->getEntityID(pixelCoords); }

float EngineCoreGL::deltaTime()
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	return m_deltaTime;
}

//=============================================================================================================================================//
//  User input.																																//
//=============================================================================================================================================//

void EngineCoreGL::mouseScrollEvent(float pixelCoords[2], float yOffset) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressLeft(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressRight(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressMiddle(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::keyEvent(int key, int action) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  Viewport.																																   //
//=============================================================================================================================================//

void EngineCoreGL::resizeEvent(float width, float height) 
{ 
	m_scene->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//