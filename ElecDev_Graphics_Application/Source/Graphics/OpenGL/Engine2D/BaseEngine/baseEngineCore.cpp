//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

// Class include.
#include "baseEngineCore.h"
// Needed to load resources.
#include "../../Resources/resource.h"

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
	
	//---------------------------
	// Create basic shader.
	//---------------------------

	std::cout << "[OPENGL][SHADERS] Compiling Basic Shader...\n";
	// Load resource from executable.
	HRSRC basicShaderResource = FindResource(getCurrentModule(), MAKEINTRESOURCE(BASIC_SHADER), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL basicResourceData = LoadResource(getCurrentModule(), basicShaderResource);
	DWORD basicResourceSize = SizeofResource(getCurrentModule(), basicShaderResource);
	char* basicResourceFinal = (char*)LockResource(basicResourceData);
	std::string basicShaderSource;
	basicShaderSource.assign(basicResourceFinal, basicResourceSize);
	m_basicShader = new Shader(basicShaderSource);

	//---------------------------
	// Create static shader.
	//---------------------------

	std::cout << "[OPENGL][SHADERS] Compiling Static Shader...\n";
	// Load resource from executable.
	HRSRC staticShaderResource = FindResource(getCurrentModule(), MAKEINTRESOURCE(STATIC_SHADER), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL staticResourceData = LoadResource(getCurrentModule(), staticShaderResource);
	DWORD staticResourceSize = SizeofResource(getCurrentModule(), staticShaderResource);
	char* staticResourceFinal = (char*)LockResource(staticResourceData);
	std::string staticShaderSource;
	staticShaderSource.assign(staticResourceFinal, staticResourceSize);
	m_staticShader = new Shader(staticShaderSource);

	//---------------------------
	// Create texture shader.
	//---------------------------

	std::cout << "[OPENGL][SHADERS] Compiling Texture Shader...\n";
	// Load resource from executable.
	HRSRC textureShaderResource = FindResource(getCurrentModule(), MAKEINTRESOURCE(TEXTURE_SHADER), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL textureResourceData = LoadResource(getCurrentModule(), textureShaderResource);
	DWORD textureResourceSize = SizeofResource(getCurrentModule(), textureShaderResource);
	char* textureResourceFinal = (char*)LockResource(textureResourceData);
	std::string textureShaderSource;
	textureShaderSource.assign(textureResourceFinal, textureResourceSize);
	m_textureShader = new Shader(textureShaderSource);
	// Done.
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

	unsigned int totVertices = 1000*1000*25;
	// Lines.
	m_linesVAO = new VertexArrayObject(GL_LINES, totVertices);
	// Triangles.
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES, totVertices);
	// Textured Triangles.
	m_textureTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, totVertices, true);
	// Background.
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES, 6);
	// Frame buffer.
	m_frameBuffer = new FrameBufferObject();

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
	// Textures & Text setup.
	//---------------------------------------------------------------------------------------

	// Enable blending for alpha channels.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_textureShader->bind();
	// Load font atlas as texture.
	m_textAtlas = loadTexture("Source\\Resources\\Fonts\\Arial_SDF.png", true);
	// Load texture for testing.
	m_texture = loadTexture("Source\\Resources\\Textures\\circuit1.png");

	// Setup shader with textures (including font atlas).
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1, 2 };
	GLCall(glUniform1iv(loc, 3, samplers));
	GLCall(glBindTextureUnit(1, m_textAtlas));	// Text Atlas.
	GLCall(glBindTextureUnit(2, m_texture));	// Testing texture.

	// Create texture renderer object.
	m_textRenderer = new TextRenderer("Source\\Resources\\Fonts\\Arial_SDF.fnt");

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
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", m_projectionMatrix);
}

//----------------------------------------------------------------------------------------------------------------------
//  EOF.
//----------------------------------------------------------------------------------------------------------------------