//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "base3DEngine.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor
Base3DEngineGL::Base3DEngineGL(stateMachine* states) : BaseEngineGL(states)
{
	// Starting log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	// The background will not be needed.
	//delete m_backgroundVAO; 

	// The projection matrix has to be recalculated for a 3D projection, using a perspective projection.
	m_projectionMatrix = glm::perspective(glm::radians(90.0f), m_imGuiViewportDimensions[0]/m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Done log.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 3D engine done.";
}

// Destructor.
Base3DEngineGL::~Base3DEngineGL() 
{
	// Call parent destructor.
	BaseEngineGL::~BaseEngineGL();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// Rendering loop.
void Base3DEngineGL::renderLoop() 
{	
	// Clear the depth buffer.  (The color buffer is already cleared by the main app)
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));

	// Call parent render loop.
	BaseEngineGL::renderLoop();
}

//=============================================================================================================================================//
//  Window events.																															   //
//=============================================================================================================================================//

void Base3DEngineGL::resizeEvent(int width, int height) 
{
	// Set new viewport values.
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;

	// Now recalculate the projection matrix.
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), m_imGuiViewportDimensions[0] / m_imGuiViewportDimensions[1], 0.1f, 100.0f);

	// Apply changes to shaders.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);
	
	// Resize the frame buffer.
	m_frameBuffer->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//