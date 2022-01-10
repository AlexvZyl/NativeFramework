//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Base3D_Engine.h"
#include "Resources/ResourceHandler.h"
#include "CoreGL/Buffers/FrameBufferObjectGL.h"
#include "CoreGL/Buffers/VertexArrayObjectGL.h"
#include "CoreGL/ShaderGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "GLM/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
Base3DEngineGL::Base3DEngineGL(GUIState* guiState, std::string contextName) 
	: EngineCoreGL(guiState, contextName)
{
	//// Starting log.
	//std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	//// Create the camera.
	//float position[3] = {0.0f, 4.0f, 6.0f};
	//float target[3] = { 0.0f, 0.0f, 0.0f };
	//m_camera = std::make_shared<Camera>(position, target, &m_viewMatrix);
	//// Init the view matrix according to the camera setup.
	//m_camera->updateView();

	//// The projection matrix has to be recalculated for a 3D projection, using a perspective projection.
	//m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0]/m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	//// Done log.
	//std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine done.";
}

// Destructor.
Base3DEngineGL::~Base3DEngineGL() 
{
	// Call core destructor.
	EngineCoreGL::~EngineCoreGL();
}

//=============================================================================================================================================//
//  Window events.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::resizeEvent(float width, float height) 
{
	//// Set new viewport values.
	//m_imGuiViewportDimensions[0] = width;
	//m_imGuiViewportDimensions[1] = height;

	//// Now recalculate the projection matrix.
	//m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0] / m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	//// Apply changes to shaders.
	//m_basicShader->bind();
	//m_basicShader->setMat4("projectionMatrix", &m_projectionMatrix);
	//m_textureShader->bind();
	//m_textureShader->setMat4("projectionMatrix", &m_projectionMatrix);
	//
	//// Resize the frame buffer.
	//m_frameBuffer->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//