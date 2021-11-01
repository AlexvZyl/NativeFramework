//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// The base 2D engine.
#include "Base3D_Engine.h"

//=============================================================================================================================================//
//  Constructor and Destructor.																												   //
//=============================================================================================================================================//

// Constructor.  Init with BaseEngineGL constructor.
Base3DEngineGL::Base3DEngineGL(stateMachine* states) : EngineCoreGL(states)
{
	// Starting log.
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 3D engine starting...";

	// Create the camera.
	float position[3] = {0.0f, 4.0f, 6.0f};
	float target[3] = { 0.0f, 0.0f, 0.0f };
	m_camera = new Camera(position, target, &m_viewMatrix);
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
	// Clear 3D engine memory.
	delete m_camera;

	// Call core destructor.
	EngineCoreGL::~EngineCoreGL();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] Rendering loop.
void Base3DEngineGL::renderLoop() 
{
	// ------------------------------------------------------------	//
	//  Setup.														//
	// ------------------------------------------------------------	//

	// Enable depth test for the 3D rendering.
	GLCall(glEnable(GL_DEPTH_TEST));

	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)m_imGuiViewportDimensions[0],
							(GLsizei)m_imGuiViewportDimensions[1]));

	// Calculate and update the engine matrices.
	m_camera->updateView();

	// Render to frame buffer.
	m_frameBuffer->bind();
	m_frameBuffer->clear();

	// ------------------------------------------------------------	//
	//  Rendering.													//
	// ------------------------------------------------------------	//

	// Draw background.
	m_backgroundShader->bind();
	// Prevent background from clipping the scene.
	GLCall(glDepthFunc(GL_EQUAL));
	m_backgroundVAO->render();
	GLCall(glDepthFunc(GL_LESS));

	// Draw basic entities.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	m_trianglesVAO->render();
	m_linesVAO->render();

	// Draw textured entities.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);
	m_texturedTrianglesVAO->render();

	// ------------------------------------------------------------	//
	//  Cleanup.													//
	// ------------------------------------------------------------	//

	// Stop rendering to the current FBO.
	m_frameBuffer->unbind();

	// Disable depth test since its not needed for the 2D engines.
	GLCall(glDisable(GL_DEPTH_TEST));

	// ------------------------------------------------------------	//
}

glm::vec4 Base3DEngineGL::pixelCoordsToWorldVector(float pixelCoords[2]) 
{
	// Find the viewpwort dimensions.
	float viewport[2] = { m_states->renderWindowSize.x, m_states->renderWindowSize.y };
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	float pixelCoordsTemp[2] = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };
	// The nomalized mouse coordinates on the users screen.
	float normalizedScreenCoords[2];
	// Apply the viewport transform the the pixels.
	normalizedScreenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	normalizedScreenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { normalizedScreenCoords[0], normalizedScreenCoords[1], -1.0f, 1.0f };
	// Apply MVP matrices.
	//m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_modelMatrix * m_viewMatrix * m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse;
	return worldVec;
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
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);
	
	// Resize the frame buffer.
	m_frameBuffer->resize(width, height);
}

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//