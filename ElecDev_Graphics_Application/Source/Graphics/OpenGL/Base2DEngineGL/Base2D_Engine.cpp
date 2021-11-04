//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Needed to load resources.
#include "../../Resources/ResourceHandler.h"
// Class include.
#include "Base2D_Engine.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
Base2DEngineGL::Base2DEngineGL(stateMachine* states) : EngineCoreGL(states)
{
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 2D engine starting...";

	// --------------------------------- //
	//  P R O J E C T I O N   S E T U P  //
	// --------------------------------- //

	// Find the minimum value of the viewport dimensions.
	float minValue;
	if (m_imGuiViewportDimensions[0] < m_imGuiViewportDimensions[1]) { minValue = m_imGuiViewportDimensions[0]; }
	else { minValue = m_imGuiViewportDimensions[1]; }
	// Scale the projection values according to the ImGUI viewport.
	float projValuesTemp[6] = { (float)-m_imGuiViewportDimensions[0] / minValue, (float)m_imGuiViewportDimensions[0] / minValue, (float)-m_imGuiViewportDimensions[1] / minValue, (float)m_imGuiViewportDimensions[1] / minValue,-1.0, 1.0 };

	// Save projection values to be used with resizing of the window.
	for (int i = 0; i < 6; i++) { m_projectionValues[i] = projValuesTemp[i]; }
	// Create projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], 0.1f, 1.0f);

	// Assign projection matrices to shader.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);

	// ------------------------------- //
	//  R E N D E R I N G   S E T U P  //
	// ------------------------------- //

	// Add a texture to the texture shader.
	m_texture = loadBitmapToGL(loadImageFromResource(CIRCUIT_TREE_PNG));
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1, 2 };
	GLCall(glUniform1iv(loc, 3, samplers));
	GLCall(glBindTextureUnit(2, m_texture));

	//---------------------------------------------------------------------------------------

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 2D engine done.";

};

// Delete and free memory.
Base2DEngineGL::~Base2DEngineGL() 
{
	EngineCoreGL::~EngineCoreGL();
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] The rendering pipeline.
void Base2DEngineGL::renderLoop()
{
	// ----------- //
	//  S E T U P  //
	// ----------- //

	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)m_imGuiViewportDimensions[0], 
							(GLsizei)m_imGuiViewportDimensions[1]));

	// Calculate and update the engine matrices.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	
	// Render to frame buffer.
	m_frameBuffer->bind();
	m_frameBuffer->clear();
	
	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //
		
	// Draw background.
	m_backgroundShader->bind();
	m_backgroundVAO->render();

	// Draw basic entities.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	m_trianglesVAO->render();
	m_linesVAO->render();

	// Draw textured entities.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);
	m_texturedTrianglesVAO->render();

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	// Stop rendering to the current FBO.
	m_frameBuffer->unbind();
}

// Return the ID to the texture that is rendered via the FBO.
unsigned int Base2DEngineGL::getRenderTexture()
{
	return m_frameBuffer->getRenderTexture();
}

glm::vec4 Base2DEngineGL::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	// Find the viewpwort dimensions.
	float viewport[2] = { m_imGuiViewportDimensions[0], m_imGuiViewportDimensions[1] };
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
	glm::vec4 screenVec = { normalizedScreenCoords[0], normalizedScreenCoords[1], 1.0f, 1.0f };
	// Apply MVP matrices.
	m_viewMatrix = m_rotationMatrix * m_translationMatrix * m_scalingMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_viewMatrix * m_projectionMatrix);//glm::inverse(m_viewMatrix) * glm::inverse(m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse;
	//worldVec.w = 1 / worldVec.w;
	//worldVec.x *= worldVec.w;
	std::cout << "rotation mat: " << m_rotationMatrix[0][0] << ',' << m_rotationMatrix[1][0] << ',' << m_rotationMatrix[2][0] << ',' << m_rotationMatrix[3][0] << std::endl;
	std::cout << m_rotationMatrix[0][1] << ',' << m_rotationMatrix[1][1] << ',' << m_rotationMatrix[2][1] << ',' << m_rotationMatrix[3][1] << std::endl;
	std::cout << m_rotationMatrix[0][2] << ',' << m_rotationMatrix[1][2] << ',' << m_rotationMatrix[2][2] << ',' << m_rotationMatrix[3][2] << std::endl;
	std::cout << m_rotationMatrix[0][3] << ',' << m_rotationMatrix[1][3] << ',' << m_rotationMatrix[2][3] << ',' << m_rotationMatrix[3][3] << std::endl;


	return worldVec;
}

void Base2DEngineGL::pixelCoordsToScreenCoords(float pixelCoords[2], float normalizedScreenCoords[2])
{
	// Find the viewpwort dimensions.
	float viewport[2] = { m_states->renderWindowSize.x, m_states->renderWindowSize.y };
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	float pixelCoordsTemp[2] = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };
	
	// Apply the viewport transform the the pixels.
	normalizedScreenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	normalizedScreenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
}

//=============================================================================================================================================//
//  Window Functions.																														   //
//=============================================================================================================================================//

// Function that handles the resizing of the ImGUI docked window.
void Base2DEngineGL::resizeEvent(float width, float height)
{
	// Calculate the value of the scaling.
	float scalingFactor[2] = { (float)width / (float)m_imGuiViewportDimensions[0], (float)height / (float)m_imGuiViewportDimensions[1] };
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;
		
	// Scale projection values.
	m_projectionValues[0] *= scalingFactor[0];
	m_projectionValues[1] *= scalingFactor[0];
	m_projectionValues[2] *= scalingFactor[1];
	m_projectionValues[3] *= scalingFactor[1];

	// Scale with the y scaling.
	m_scalingMatrix = glm::scale(m_scalingMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));
	// Update base matrix.
	m_scalingMatrixBase = glm::scale(m_scalingMatrixBase, glm::vec3(scalingFactor[1], scalingFactor[1], 1.0f));

	// Create new projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);

	// Apply changes to shaders.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);

	// Resize FBO attachments.
	m_frameBuffer->resize(width, height);

	// Change viewport dimmensions.
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;
}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//