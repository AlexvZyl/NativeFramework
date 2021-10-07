//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "baseEngineCore.h"
// Needed to load resources.
#include "../../Resources/resource.h"

#include "Misc/stb_image.h"


//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// With GLFW window.
BaseEngineGL::BaseEngineGL(stateMachine* states)
	:m_states(states)
{
	//---------------------------------------------------------------------------------------
	// Setup shaders.
	//---------------------------------------------------------------------------------------
	
	std::cout << "[OPENGL][BASE ENGINE] Compiling shaders...\n";

	//---------------------------
	// Create basic shader.
	//---------------------------
	// Load shader code from the executable.
	std::string basicShaderSource = loadShaderFromResource(BASIC_SHADER);
	m_basicShader = new Shader(basicShaderSource);

	//---------------------------
	// Create static shader.
	//---------------------------
	// Load shader code from the executable.
	std::string staticShaderSource = loadShaderFromResource(STATIC_SHADER);
	m_staticShader = new Shader(staticShaderSource);

	//---------------------------
	// Create texture shader.
	//---------------------------
	// Load resource from executable.
	std::string textureShaderSource = loadShaderFromResource(TEXTURE_SHADER);
	m_textureShader = new Shader(textureShaderSource);

	// Done.
	std::cout << "[OPENGL][BASE ENGINE] Shaders done.\n\n";

	//---------------------------------------------------------------------------------------
	// Windows setup.
	//---------------------------------------------------------------------------------------

	// ImGUI viewport.
	float viewportImGUI[2] = {500,500};
	m_imGuiViewportDimensions[0] = viewportImGUI[0];
	m_imGuiViewportDimensions[1] = viewportImGUI[1];

	//---------------------------------------------------------------------------------------
	// Matrices setup.
	//---------------------------------------------------------------------------------------

	// Find the minimum value of the viewport dimensions.
	float minValue;
	if (viewportImGUI[0] < viewportImGUI[1]) { minValue = viewportImGUI[0]; }
	else { minValue = viewportImGUI[1]; }
	// Scale the projection values according to the ImGUI viewport.
	float projValuesTemp[6] = { (float)-viewportImGUI[0] / minValue, (float)viewportImGUI[0] / minValue, (float)-viewportImGUI[1] / minValue, (float)viewportImGUI[1] / minValue,-1.0, 1.0 };

	// Save projection values to be used with resizing of the window.
	for (int i = 0; i < 6; i++) { m_projectionValues[i] = projValuesTemp[i]; }
	// Create projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], -1.0f, 1.0f);

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
	// Buffers setup.
	//---------------------------------------------------------------------------------------

	unsigned int totVertices = 1000*100;
	// Lines.
	m_linesVAO = new VertexArrayObject(GL_LINES, totVertices);
	// Triangles.
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES, totVertices);
	// Textured Triangles.
	m_textureTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, totVertices, true);
	// Background.
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES, 6);
	// Frame buffer.
	m_frameBuffer = new FrameBufferObject((int)m_imGuiViewportDimensions[0], (int)m_imGuiViewportDimensions[1]);

	//---------------------------------------------------------------------------------------
	// Background setup.
	//---------------------------------------------------------------------------------------

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

	//---------------------------------------------------------------------------------------
	// Textures & Text setup.
	//---------------------------------------------------------------------------------------

	// Enable blending for alpha channels.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_textureShader->bind();
	// Load texture for testing.
	m_texture = loadTexture("Source\\Resources\\Textures\\circuitTree.png");

	// Create texture renderer object.
	m_textRenderer = new TextRenderer(ARIAL_SDF_FNT, ARIAL_SDF_BMP);

	// Setup shader with textures (including font atlas).
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1, 2 };
	GLCall(glUniform1iv(loc, 3, samplers));
	GLCall(glBindTextureUnit(1, m_textRenderer->m_textureID));	// Text Atlas.
	GLCall(glBindTextureUnit(2, m_texture));	// Testing texture.

	//---------------------------------------------------------------------------------------


};

// Delete and free memory.
BaseEngineGL::~BaseEngineGL() 
{
	// Delete shaders.
	delete m_basicShader;
	delete m_staticShader;
	delete m_textureShader;
	// Delete VAO's.
	// Lines.
	delete m_linesVAO;
	// Background.
	delete m_backgroundVAO;
	// Triangles.
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

void BaseEngineGL::renderLoop()
{
	// Set glViewport for the ImGUI context.
	GLCall(glViewport(0, 0, (GLsizei)m_imGuiViewportDimensions[0], (GLsizei)m_imGuiViewportDimensions[1]));

	// Render to frame buffer.
	m_frameBuffer->bind();

	//---------------------------------------------------------------------------------------
	// Matrix calculations.
	//---------------------------------------------------------------------------------------
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	
	//---------------------------------------------------------------------------------------
	// Draw static entities.
	//---------------------------------------------------------------------------------------
	m_staticShader->bind();
	m_backgroundVAO->render();

	//---------------------------------------------------------------------------------------
	// Draw basic entities.
	//---------------------------------------------------------------------------------------
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	m_linesVAO->render();
	m_trianglesVAO->render();

	//---------------------------------------------------------------------------------------
	// Draw textured entities.
	//---------------------------------------------------------------------------------------
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);
	m_textureTrianglesVAO->render();

	//---------------------------------------------------------------------------------------

	// Do not continue rendering to a frame buffer.
	m_frameBuffer->unbind();
}

// Return the ID to the texture that is rendered via the FBO.
unsigned int BaseEngineGL::getRenderedTexID() 
{
	return m_frameBuffer->getTexID();
}

//----------------------------------------------------------------------------------------------------------------------
//  Coordinate systems.
//----------------------------------------------------------------------------------------------------------------------

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 BaseEngineGL::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	// The coordinates on the screen.
	float screenCoords[2];  
	// Find the viewpwort dimensions.
	float viewport[2] = {m_states->renderWindowSize.x, m_states->renderWindowSize.y};
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	float pixelCoordsTemp[2] = { pixelCoords[0] , (float)viewport[1] - pixelCoords[1] };
	
	// Apply the viewport transform the the pixels.
	screenCoords[0] = (pixelCoordsTemp[0] - viewportOffset[0] / 2) / (viewportOffset[0] / 2);
	screenCoords[1] = (pixelCoordsTemp[1] - viewportOffset[1] / 2) / (viewportOffset[1] / 2);
	// Convert to screen vector.
	glm::vec4 screenVec = { screenCoords[0], screenCoords[1], 0, 1 };

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
//  Camera manipulation.
//----------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------