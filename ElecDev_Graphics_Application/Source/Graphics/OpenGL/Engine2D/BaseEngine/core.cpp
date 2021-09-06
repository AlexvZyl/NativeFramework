/*
This is where the drawing enigine is handled.  This is only used to draw elements to the screen.
The interactive engine (the one where elements can be drawn is handled in designEngine).
*/

//----------------------------------------------------------------------------------------------------------------------
//  Includes.
//----------------------------------------------------------------------------------------------------------------------

#include "core.h"
#include <numbers>

//----------------------------------------------------------------------------------------------------------------------
//  Constructor & Destructor.
//----------------------------------------------------------------------------------------------------------------------

// With GLFW window.
BaseEngineGL::BaseEngineGL(GLFWwindow* window)
	:m_window(window)
{	
	// Create basic shader.
	std::string basicShaderPath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\basicShader.shader";
	m_basicShader = new Shader(basicShaderPath);
	// Create static shader.
	std::string staticShaderPath = "Source\\Graphics\\OpenGL\\ShaderHandler\\Source\\staticShader.shader";
	m_staticShader = new Shader(staticShaderPath);

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
	float projValuesTemp[6] = {(float)-viewport[0]/minValue, (float)viewport[0]/minValue, (float)-viewport[1]/minValue, (float)viewport[1]/minValue,-1.0, 1.0 };
	// Save projection values to be used with resizing of the window.
	for (int i = 0; i < 6; i++) { m_projectionValues[i] = projValuesTemp[i]; }
	// Create projection matrix.
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], -1.0f, 1.0f);

	// Assign matrices to shader.
	m_basicShader->bind();
	m_basicShader->setMat4("worldMatrix", m_modelMatrix);
	m_basicShader->setMat4("projectionMatrix", m_projectionMatrix);
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);

	//---------------------------------------------------------------------------------------
	//  Mouse point setup.
	//---------------------------------------------------------------------------------------

	float mouseColor[4] = {1.0f, 0.0f, 0.0f, 1.0f };
	m_mousePoint = new MousePoint(m_window, mouseColor, 0.01);

	//---------------------------------------------------------------------------------------
	// Buffers setup.
	//---------------------------------------------------------------------------------------

	int size = 1000;
	// Lines.
	m_linesVAO = new VertexArrayObject(BufferType::LINES, size);
	// Background.
	m_backgroundVAO = new VertexArrayObject(BufferType::QUAD_FILLED, 8);
	// Triangles.
	m_trianglesClearVAO = new VertexArrayObject(BufferType::TRIANGLE_CLEAR, size);
	m_trianglesFilledVAO = new VertexArrayObject(BufferType::TRIANGLE_FILLED, size);
	// Quads.
	m_quadsClearVAO = new VertexArrayObject(BufferType::QUAD_CLEAR, size);
	m_quadsFilledVAO = new VertexArrayObject(BufferType::QUAD_FILLED, size);
	// Circles.
	m_circlesClearVAO = new VertexArrayObject(BufferType::CIRCLE_CLEAR, size * m_circleResolution);
	m_circlesFilledVAO = new VertexArrayObject(BufferType::CIRCLE_FILLED, size*m_circleResolution);

	//---------------------------------------------------------------------------------------
	// Background. setup.
	//---------------------------------------------------------------------------------------

	// Assign background data.
	float bgColor1[4] = { (float)162 / 255, (float)184 / 255, (float)242 / 255, 1.0f };
	float bgColor2[4] = { (float)209 / 255, (float)219 / 255, (float)249 / 255, 1.0f };
	VertexData v5(1.0f, 1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	// Top right.
	VertexData v6(-1.0f, 1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Top left.
	VertexData v7(-1.0f, -1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3]);	//  Bottom left.
	VertexData v8(1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3]);	//  Bottom right.
	// Create background.
	m_backgroundVAO->writeData(v5, v6, v7, v8);

};

// Delete and free memory.
BaseEngineGL::~BaseEngineGL() 
{
	// Delete shaders.
	delete m_basicShader;
	delete m_staticShader;
	// Delete VAO's.
	delete m_linesVAO;
	delete m_backgroundVAO;
	delete m_trianglesClearVAO;
	delete m_trianglesFilledVAO;
	delete m_quadsClearVAO;
	delete m_quadsFilledVAO;
	delete m_circlesClearVAO;
	delete m_circlesFilledVAO;
	// Delete others.
	delete m_mousePoint;
}

//----------------------------------------------------------------------------------------------------------------------
//  Rendering.
//----------------------------------------------------------------------------------------------------------------------

void BaseEngineGL::renderLoop()
{
	// Draw background.
	m_staticShader->bind();
	m_backgroundVAO->render();

	// Apply camera movements to shader(s).
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	// Bind basic shader and apply changes.
	m_basicShader->bind();
	m_basicShader->setMat4("viewMatrix", m_viewMatrix);
	// Lines.
	m_linesVAO->render();
	// Triangles.
	m_trianglesClearVAO->render();
	m_trianglesFilledVAO->render();
	// Quads.
	m_quadsClearVAO->render();
	m_quadsFilledVAO->render();
	// Circles.
	m_circlesFilledVAO->render();
	m_circlesClearVAO->render();

	// Update and draw mouse point.
	//m_staticShader->bind();
	//double coords[2];
	//glfwGetCursorPos(m_window, &coords[0], &coords[1]);
	//glm::vec4 mouseWorldCoords = pixelCoordsToWorldCoords(coords);
	//float drawCoords[2] = { mouseWorldCoords[0],  mouseWorldCoords[1] };
	//m_mousePoint->render(drawCoords);
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