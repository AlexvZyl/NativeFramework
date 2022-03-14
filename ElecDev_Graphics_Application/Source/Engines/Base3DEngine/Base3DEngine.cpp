//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "External/Misc/ConsoleColor.h"
#include "GLM/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engines/EngineCore/EngineCore.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "OpenGL/Renderer/RendererGL.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
// Constructor.  Init with BaseEngineGL constructor.
Base3DEngine::Base3DEngine() 
	: EngineCore()
{
	// Create and bind a 3D scene.
	m_scene = std::make_unique<Scene>(CameraType::Standard3D, 500, 500);
	Renderer::bindScene(m_scene.get());
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//