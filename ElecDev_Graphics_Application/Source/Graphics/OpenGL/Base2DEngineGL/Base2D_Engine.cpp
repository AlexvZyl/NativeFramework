//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Needed to load resources.
#include "../../Resources/ResourceHandler.h"
// Class include.
#include "Base2D_Engine.h"
#include "CoreGL/Buffers/FrameBufferObjectGL.h"
#include "CoreGL/Buffers/VertexArrayObjectGL.h"
#include "CoreGL/ShaderGL.h"
#include "CoreGL/FontsGL.h"
#include "CoreGL/SceneGL.h"
#include "CoreGL/CameraGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "glm/gtc/matrix_transform.hpp"

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