//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Needed to load resources.
#include "../../Resources/ResourceHandler.h"
// Class include.
#include "Base2D_Engine.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// With GLFW window.
BaseEngineGL::BaseEngineGL(stateMachine* states)
	:m_states(states)
{
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 2D engine starting...";

	//---------------------------------------------------------------------------------------
	// Compile shaders.
	//---------------------------------------------------------------------------------------

	m_basicShader = new Shader(BASIC_SHADER);
	m_staticShader = new Shader(STATIC_SHADER);
	m_textureShader = new Shader(TEXTURE_SHADER);

	//---------------------------------------------------------------------------------------
	// Matrices setup.
	//---------------------------------------------------------------------------------------

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

	// Assign matrices to basic shader.
	m_basicShader->bind();
	m_basicShader->setMat4("worldMatrix", m_modelMatrix);
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	// Assign matrices to texture shader.
	m_textureShader->bind();
	m_textureShader->setMat4("worldMatrix", m_modelMatrix);
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);

	//---------------------------------------------------------------------------------------
	// Rendering setup.
	//---------------------------------------------------------------------------------------

	// Create the background for the scene.
	createBackground();

	// Buffers setup.
	m_linesVAO = new VertexArrayObject(GL_LINES);					
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES);
	m_textureTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, true);
	m_frameBuffer = new FrameBufferObject((int)m_imGuiViewportDimensions[0], (int)m_imGuiViewportDimensions[1], 8);

	// Textures.
	m_texture = loadBitmapToGL(loadImageFromResource(CIRCUIT_TREE_PNG));	// Testing texture.
	m_textRenderer = new TextRenderer(ARIAL_SDF_FNT, ARIAL_SDF_PNG);		// Text renderer.

	// Setup shader with textures (including font atlas).
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1, 2 };
	GLCall(glUniform1iv(loc, 3, samplers));
	GLCall(glBindTextureUnit(1, m_textRenderer->m_textureID));	// Text Atlas.
	GLCall(glBindTextureUnit(2, m_texture));					// Testing texture.

	//---------------------------------------------------------------------------------------

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 2D engine done.";

};

// Delete and free memory.
BaseEngineGL::~BaseEngineGL() 
{
	// Delete shaders.
	delete m_basicShader;
	delete m_staticShader;
	delete m_textureShader;
	// Delete VAO's.
	delete m_linesVAO;
	delete m_backgroundVAO;
	delete m_trianglesVAO;
	delete m_textureTrianglesVAO;
	// Delete text renderer.
	delete m_textRenderer;
	// FBO
	delete m_frameBuffer;
}

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

// [MAIN LOOP] The rendering pipeline.
void BaseEngineGL::renderLoop()
{
	// ------------------------------------------------------------	//
	//  Setup.														//
	// ------------------------------------------------------------	//

	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)m_imGuiViewportDimensions[0], 
							(GLsizei)m_imGuiViewportDimensions[1]));

	// Calculate and update the engine matrices.
	updateMartrices();

	// Render to frame buffer.
	m_frameBuffer->bind();
	m_frameBuffer->clear();
	
	// ------------------------------------------------------------	//
	//  Rendering.													//
	// ------------------------------------------------------------	//
		
	// Draw static entities.
	m_staticShader->bind();
	m_backgroundVAO->render();

	// Draw basic entities.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	m_trianglesVAO->render();
	m_linesVAO->render();

	// Draw textured entities.
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);
	m_textureTrianglesVAO->render();

	// ------------------------------------------------------------	//
	//  Cleanup.													//
	// ------------------------------------------------------------	//

	// Stop rendering to the current FBO.
	m_frameBuffer->unbind();

	// ------------------------------------------------------------	//
}

// Applies the render loop matrix calculations.
void BaseEngineGL::updateMartrices() 
{
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
}

// Return the ID to the texture that is rendered via the FBO.
unsigned int BaseEngineGL::getRenderTexture()
{
	return m_frameBuffer->getRenderTexture();
}

//----------------------------------------------------------------------------------------------------------------------

// Used to create a background.
// Is a virtual function so that child engines have the option to change the background.
void BaseEngineGL::createBackground() 
{
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES);
	// Assign background data.
	float bgColor1[4] = { (float)162 / 255, (float)184 / 255, (float)242 / 255, 1.0f };
	float bgColor2[4] = { (float)210 / 255, (float)242 / 255, (float)255 / 255, 1.0f };
	VertexData v5(1.0f, 1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	//  Top right.
	VertexData v6(-1.0f, 1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Top left.
	VertexData v7(-1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom left.
	VertexData v8(1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom right.
	std::vector<VertexData> vertices = { v5, v6, v7, v7, v8, v5 };
	// Create background.
	m_backgroundVAO->writeData(vertices);
	m_backgroundVAO->updateGPU();
}

//----------------------------------------------------------------------------------------------------------------------
//  Coordinate systems.
//----------------------------------------------------------------------------------------------------------------------

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 BaseEngineGL::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	
	// Find the viewpwort dimensions.
	float viewport[2] = {m_states->renderWindowSize.x, m_states->renderWindowSize.y};
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
	glm::vec4 screenVec = { normalizedScreenCoords[0], normalizedScreenCoords[1], 0, 1 };

	// Apply MVP matrices.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_modelMatrix * m_viewMatrix * m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse ;

	return worldVec;
}

//----------------------------------------------------------------------------------------------------------------------
//  Window functions.
//----------------------------------------------------------------------------------------------------------------------

// Function that handles the resizing of the ImGUI docked window.
void BaseEngineGL::resizeEvent(int width, int height)
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

	// Resize FBO texture.
	m_frameBuffer->resize(width, height);

	// Change viewport dimmensions.
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------