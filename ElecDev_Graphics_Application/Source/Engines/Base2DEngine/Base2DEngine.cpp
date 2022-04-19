//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "Engines/Base2DEngine/Base2DEngine.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "OpenGL/Renderer/RendererGL.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
Base2DEngine::Base2DEngine() 
	: EngineCore()
{
	m_scene = std::make_unique<Scene>(CameraType::Standard2D, glm::vec2(500, 500));
};

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//