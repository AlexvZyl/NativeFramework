//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

#include <iostream>
#include "Misc/ConsoleColor.h"
#include "GLM/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Engines/EngineCore/EngineCore.h"
#include "Engines/Base3DEngine/Base3DEngine.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
Base3DEngine::Base3DEngine() 
	: EngineCore()
{
	// Starting log.
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	//// Create the camera.
	//float position[3] = {0.0f, 4.0f, 6.0f};
	//float target[3] = { 0.0f, 0.0f, 0.0f };
	//m_camera = std::make_shared<Camera>(position, target, &m_viewMatrix);
	//// Init the view matrix according to the camera setup.
	//m_camera->updateView();

	//// The projection matrix has to be recalculated for a 3D projection, using a perspective projection.
	//m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0]/m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Done log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine done.";
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//