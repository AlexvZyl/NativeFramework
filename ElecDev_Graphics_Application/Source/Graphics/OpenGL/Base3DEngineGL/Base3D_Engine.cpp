//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Base3D_Engine.h"
#include "Utilities/Resources/ResourceHandler.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/ShaderHandlerGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "GLM/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Peripherals/CameraGL.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
Base3DEngineGL::Base3DEngineGL(GUIState* guiState, std::string contextName) 
	: EngineCoreGL(guiState, contextName)
{
	// Starting log.
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	// Create the background shader.
	m_backgroundShader = std::make_unique<Shader>(BACKGROUND_SHADER_3D);

	// Create the camera.
	float position[3] = {0.0f, 4.0f, 6.0f};
	float target[3] = { 0.0f, 0.0f, 0.0f };
	m_camera = std::make_shared<Camera>(position, target, &m_viewMatrix);
	// Init the view matrix according to the camera setup.
	m_camera->updateView();

	// The projection matrix has to be recalculated for a 3D projection, using a perspective projection.
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0]/m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Done log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine done.";
}

// Destructor.
Base3DEngineGL::~Base3DEngineGL() 
{
	// Call core destructor.
	EngineCoreGL::~EngineCoreGL();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] Rendering loop.
void Base3DEngineGL::renderLoop() 
{
	// ---------- //
	//  S E T U P //
	// ---------- //

	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)m_imGuiViewportDimensions[0],
							(GLsizei)m_imGuiViewportDimensions[1]));

	// Calculate and update the engine matrices.
	m_camera->updateView();

	// Render to frame buffer.
	m_frameBuffer->bind();
	m_frameBuffer->clear();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// Draw background.
	m_backgroundShader->bind();
	// Prevent background from clipping the scene.
	GLCall(glDepthFunc(GL_EQUAL));
	m_backgroundVAO->render();
	GLCall(glDepthFunc(GL_LESS));

	// Draw basic entities.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", &m_viewMatrix);
	m_trianglesVAO->render();
	m_linesVAO->render();

	// Draw textured entities.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", &m_viewMatrix);
	m_texturedTrianglesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	m_frameBuffer->unbind();
}

//=============================================================================================================================================//
//  Window events.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::resizeEvent(float width, float height) 
{
	// Set new viewport values.
	m_imGuiViewportDimensions[0] = width;
	m_imGuiViewportDimensions[1] = height;

	// Now recalculate the projection matrix.
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0] / m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Apply changes to shaders.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", &m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", &m_projectionMatrix);
	
	// Resize the frame buffer.
	m_frameBuffer->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//