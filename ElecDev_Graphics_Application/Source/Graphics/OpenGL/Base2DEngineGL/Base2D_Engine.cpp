//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Needed to load resources.
#include "../../Resources/ResourceHandler.h"
// Class include.
#include "Base2D_Engine.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/ShaderHandlerGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "glm/gtc/matrix_transform.hpp"
#include "CoreGL/Fonts.h"
#include "CoreGL/Scene.h"
#include "CoreGL/Camera.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
Base2DEngineGL::Base2DEngineGL(GUIState* guiState, std::string contextName) 
	: EngineCoreGL(guiState, contextName)
{
	// Start.
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 2D engine starting...";

	// Create scene.
	m_scene = std::make_unique<Scene>(CameraType::Standard2D, 500, 500);

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 2D engine done.";

};

// Delete and free memory.
Base2DEngineGL::~Base2DEngineGL() {}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//