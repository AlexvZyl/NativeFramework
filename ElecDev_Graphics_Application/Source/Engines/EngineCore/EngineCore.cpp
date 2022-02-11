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

#include <iostream>
#include <memory>
#include "OpenGL/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "Engines/EngineCore/EngineCore.h"
#include "External/GLFW/Includes/GLFW/glfw3.h"

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void EngineCore::onRender()
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
//  EOF.																																	   //
//=============================================================================================================================================//