/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "core.h"

//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// With GLFW window.
BaseEngineGL::BaseEngineGL(GLFWwindow* window)
	:m_window(window)
{
	//---------------------------------------------------------------------------------------
	// Setup shaders.
	//---------------------------------------------------------------------------------------
	 
	// Create basic shader.
	std::string basicShaderPath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\basicShader.shader";
	std::cout << "[OPENGL][SHADERS] Compiling Basic Shader...\n";
	m_basicShader = new Shader(basicShaderPath);
	// Create static shader.
	std::string staticShaderPath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\staticShader.shader";
	std::cout << "[OPENGL][SHADERS] Compiling Static Shader...\n";
	m_staticShader = new Shader(staticShaderPath);
	// Create texture shader.
	std::string textureShaderPath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\textureShader.shader";
	std::cout << "[OPENGL][SHADERS] Compiling Texture Shader...\n";
	m_textureShader = new Shader(textureShaderPath);
	std::cout << "[OPENGL][SHADERS] Done.\n\n";

	//---------------------------------------------------------------------------------------
	// Matrices setup.
	//---------------------------------------------------------------------------------------

	// Find the viewpwort dimensions and store it.
	int viewport[2];
	glfwGetWindowSize(m_window, &viewport[0], &viewport[1]);
	// Store the value to use when viewport changes.
	m_viewportDimensions[0] = viewport[0];
	m_viewportDimensions[1] = viewport[1];

	// Find the minimum value of the viewport dimensions.
	int minValue;
	if (viewport[0] < viewport[1]) { minValue = viewport[0]; }
	else { minValue = viewport[1]; }
	// Scale the projection values according to the viewport aspect ratio.
	float projValuesTemp[6] = { (float)-viewport[0] / minValue, (float)viewport[0] / minValue, (float)-viewport[1] / minValue, (float)viewport[1] / minValue,-1.0, 1.0 };
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

	int size = 1000*1000*10;
	// Lines.
	m_linesVAO = new VertexArrayObject(GL_LINES, size);
	// Triangles.
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES, size);
	// Textured Triangles.
	m_texTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, size, true);
	// Background.
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES, 6);

	//---------------------------------------------------------------------------------------
	// Background setup.
	//---------------------------------------------------------------------------------------

	// Assign background data.
	float bgColor1[4] = { (float)162 / 255, (float)184 / 255, (float)242 / 255, 1.0f };
	float bgColor2[4] = { (float)230 / 255, (float)240 / 255, (float)255 / 255, 1.0f };
	VertexData v5(1.0f, 1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	// Top right.
	VertexData v6(-1.0f, 1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Top left.
	VertexData v7(-1.0f, -1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	//  Bottom left.
	VertexData v8(1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom right.
	std::vector<VertexData> vertices = { v5, v6, v7, v7, v8, v5 };
	// Create background.
	m_backgroundVAO->writeData(vertices);

	//---------------------------------------------------------------------------------------
	// Textures Setup.
	//---------------------------------------------------------------------------------------

	std::string texPath = "Source\\Graphics\\OpenGL\\Engine2D\\BaseEngine\\Textures\\circuit1.png";
	m_texture = loadTexture(texPath);
	TexturedVertexData v1(1.25f, 1.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	TexturedVertexData v2(1.25f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f);
	TexturedVertexData v3(0.75f, 0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	TexturedVertexData v4(0.75f, 1.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	std::vector<TexturedVertexData> verticesTex = { v1, v2, v3, v3, v4, v1 };
	m_texTrianglesVAO->writeData(verticesTex);

	// Setup shader with textures.
	m_textureShader->bind();
	auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

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
	delete m_texTrianglesVAO;
}

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

void BaseEngineGL::renderLoop()
{
	//---------------------------------------------------------------------------------------
	// Draw background.
	//---------------------------------------------------------------------------------------
	m_staticShader->bind();
	m_backgroundVAO->render();

	//---------------------------------------------------------------------------------------
	// Matrix calculations.
	//---------------------------------------------------------------------------------------
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;

	//---------------------------------------------------------------------------------------
	// Basic rendering.
	//---------------------------------------------------------------------------------------
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	// Lines.
	m_linesVAO->render();
	// Triangles.
	m_trianglesVAO->render();

	//---------------------------------------------------------------------------------------
	// Textured rendering.
	//---------------------------------------------------------------------------------------
	m_textureShader->bind();
	m_textureShader->setMat4("viewMatrix", m_viewMatrix);
	// Bind textures.
	GLCall(glBindTextureUnit(1, m_texture));
	m_texTrianglesVAO->render();

	//---------------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------------------------------------------
//  Coordinate systems.
//----------------------------------------------------------------------------------------------------------------------

// Function that takes pixel coordinates as input and return the coordinates in the world.
glm::vec4 BaseEngineGL::pixelCoordsToWorldCoords(double pixelCoords[2])
{
	// The coordinates on the screen.
	double screenCoords[2];  
	// Find the viewpwort dimensions.
	int viewport[2];
	glfwGetWindowSize(m_window, &viewport[0], &viewport[1]);
	// Account for pixel offset.
	float viewportOffset[2] = { (float)viewport[0], (float)viewport[1] };
	// OpenGL places the (0,0) point in the top left of the screen.  Place it in the bottom left cornder.
	double pixelCoordsTemp[2] = { pixelCoords[0] , (double)viewport[1] - pixelCoords[1] };
	
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

// Function that handles engine resizing.
// Viewport changes are made in the main Applicatioon since it affects everything.
void BaseEngineGL::resizeEvent(int width, int height)
{
	// Calculate the value of the scaling.
	double scalingFactor[2] = { (double)width / (double)m_viewportDimensions[0], (double)height / (double)m_viewportDimensions[1] };
	m_viewportDimensions[0] = width;
	m_viewportDimensions[1] = height;
		
	// Scale projection values.
	m_projectionValues[0] *= scalingFactor[0];
	m_projectionValues[1] *= scalingFactor[0];
	m_projectionValues[2] *= scalingFactor[1];
	m_projectionValues[3] *= scalingFactor[1];

	// Arrange order of sacling based on if it should be division or multiplication.
	if (scalingFactor[1] < 1)  //  Minimizing.
	{
		// Create new projection matrix.
		m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);
	
	}
	else  //  Maximizing.
	{
		// Create new projection matrix.
		m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);
		// Scale the drawing so that it stays the same size relative to the viewport.
		m_projectionMatrix = glm::scale(m_projectionMatrix, glm::vec3(scalingFactor[1], scalingFactor[1], 1));
	}

	// Apply changes to shaders.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------