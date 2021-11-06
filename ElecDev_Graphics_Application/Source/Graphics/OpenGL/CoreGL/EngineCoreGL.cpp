/*
All of the CoreEngineGL functions.  Some are general enough to be implemented here.
The rest of the functions are going to be implementd in the respective engines.
If a function is called that is not implemented in the child engine an error is 
thrown via functionNotImplementedError().  This also makes it easier to catch errors
and notify the user via the terminal interface.
*/

//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Class include.
#include "EngineCoreGL.h"
#include "Resources/ResourceHandler.h"
#include "GLFW/glfw3.h"

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
EngineCoreGL::EngineCoreGL(GUIState* guiState)
	:m_guiState(guiState)
{
	// Print start message.
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Engine core starting...";

	// ----------------------------------------- //
	//  C R E A T E   B A S I C   S H A D E R S  //
	// ----------------------------------------- //

	// Compile the shaders, using the resources embedded in the exe.
	m_basicShader = new Shader(BASIC_SHADER);
	m_backgroundShader = new Shader(BACKGROUND_SHADER);
	m_textureShader = new Shader(TEXTURE_SHADER);

	// Set default values for the shaders.  The background shader does not require
	// this setup, since it does not work with the MVP matrices.
	glm::mat4 identity = glm::mat4(1.0f);
	m_basicShader->bind();
	m_basicShader->setMat4("worldMatrix", identity);
	m_basicShader->setMat4("projectionMatrix", identity);
	m_basicShader->setMat4("viewMatrix", identity);
	m_textureShader->bind();
	m_textureShader->setMat4("worldMatrix", identity);
	m_textureShader->setMat4("projectionMatrix", identity);
	m_textureShader->setMat4("viewMatrix", identity);

	// ------------------------------------- //
	//  C R E A T E   B A S I C   V A O ' S  //
	// ------------------------------------- //

	m_linesVAO = new VertexArrayObject(GL_LINES);
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES);
	m_texturedTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, true);
	m_frameBuffer = new FrameBufferObject((int)m_imGuiViewportDimensions[0], (int)m_imGuiViewportDimensions[1], 8);
	createDefaultBackground();

	// ----------------------------------------- //
	//  C R E A T E   T E X T   R E N D E R E R  //
	// ----------------------------------------- //

	m_textRenderer = new TextRenderer(ARIAL_SDF_FNT, ARIAL_SDF_PNG);
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1 };
	GLCall(glUniform1iv(loc, 2, samplers));
	GLCall(glBindTextureUnit(1, m_textRenderer->m_textureID));	// Text Atlas.

	// Print done message.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core done.\n";
}

// Destructor.
EngineCoreGL::~EngineCoreGL()
{
	delete m_basicShader;			// Shader. 
	delete m_backgroundShader;		// "
	delete m_textureShader;			// "
	delete m_linesVAO;				// VAO.
	delete m_backgroundVAO;			// "
	delete m_trianglesVAO;			// "
	delete m_texturedTrianglesVAO;	// "
	delete m_textRenderer;			// Text renderer.
	delete m_frameBuffer;			// FBO.
}

//=============================================================================================================================================//
//  Errors.																																	   //
//=============================================================================================================================================//

void EngineCoreGL::functionNotImplementedError(std::string functionName) 
{
	std::cout << red << "\n[OPENGL] [ERROR] : " << white << "'" << functionName << "' is not implemented for the engine type.\n";
}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

void EngineCoreGL::renderLoop() { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::autoCenter() { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::drawDemo(unsigned int loopCount) { functionNotImplementedError(__FUNCTION__); }

unsigned int EngineCoreGL::getRenderTexture() { return m_frameBuffer->getRenderTexture(); }

glm::vec3 EngineCoreGL::pixelCoordsToWorldCoords(float pixelCoords[2])
{
	glm::vec4 viewPort = { 0.0f, 0.0f, m_imGuiViewportDimensions[0], m_imGuiViewportDimensions[1] };
	glm::vec3 pixelCoords3 = { pixelCoords[0], viewPort[3] - pixelCoords[1], 0.0f };
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::vec3 worldVec = glm::unProject(pixelCoords3, m_viewMatrix, m_projectionMatrix, viewPort);
	return worldVec;
}

glm::vec3 EngineCoreGL::pixelCoordsToCameraCoords(float pixelCoords[2])
{
	// Find the viewpwort dimensions.
	float viewport[2] = { m_guiState->renderWindowSize[0], m_guiState->renderWindowSize[1]};
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
	glm::vec4 screenVec = { normalizedScreenCoords[0], normalizedScreenCoords[1], 0.0f, 1.0f };
	// Apply MVP matrices.
	m_viewMatrix = m_scalingMatrix * m_rotationMatrix * m_translationMatrix;
	glm::mat4 MVPinverse = glm::inverse(m_modelMatrix * m_viewMatrix * m_projectionMatrix);
	glm::vec4 worldVec = screenVec * MVPinverse;
	return worldVec;
}

unsigned int EngineCoreGL::getEntityID(float pixelCoords[2]) 
{ 
	// Adjust the pixel coords.
	float pixelCoordsTemp[2] = { pixelCoords[0], m_imGuiViewportDimensions[1] - pixelCoords[1] };
	return m_frameBuffer->getEntityID(pixelCoordsTemp);
}

void EngineCoreGL::updateGPU() 
{
	m_linesVAO->updateGPU();
	m_trianglesVAO->updateGPU();
	m_texturedTrianglesVAO->updateGPU();
}																								

 void EngineCoreGL::createDefaultBackground() 
 {
	 // Create the VAO.
	 m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES);
	 // Assign background data.
	 float bgColor1[4] = { (float)162 / 255, (float)184 / 255, (float)242 / 255, 1.0f };
	 float bgColor2[4] = { (float)210 / 255, (float)242 / 255, (float)255 / 255, 1.0f };
	 VertexData v5(1.0f, 1.0f, 0.0f, bgColor2[0], bgColor2[1], bgColor2[2], bgColor2[3], 0);	//  Top right.
	 VertexData v6(-1.0f, 1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3], 0);	//  Top left.
	 VertexData v7(-1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3], 0);	//  Bottom left.
	 VertexData v8(1.0f, -1.0f, 0.0f, bgColor1[0], bgColor1[1], bgColor1[2], bgColor1[3], 0);	//  Bottom right.
	 std::vector<VertexData> vertices = { v5, v6, v7, v7, v8, v5 };
	 // Create background.
	 m_backgroundVAO->appendDataCPU(vertices);
	 m_backgroundVAO->updateGPU();
 }

 float EngineCoreGL::deltaTime()
 {
	 float currentFrame = glfwGetTime();
	 m_deltaTime = currentFrame - m_lastFrame;
	 m_lastFrame = currentFrame;
	 return m_deltaTime;
 }

 //=============================================================================================================================================//
 //  Mouse handling.																															//
 //=============================================================================================================================================//

 void EngineCoreGL::mouseScrollEvent(float pixelCoords[2], float yOffset) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::mousePressLeft(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::mousePressRight(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::mousePressMiddle(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  2D API.																																	   //
//=============================================================================================================================================//

 void EngineCoreGL::drawLine(float position1[2], float position2[2], float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawTriangleClear(float position1[2], float position2[2], float position3[2], float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawTriangleFilled(float position1[2], float position2[2], float position3[2], float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawQuadClear(float position[2], float width, float height, float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawQuadFilled(float position[2], float width, float height, float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawCircleClear(float position[2], float radius, float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawCircleFilled(float position[2], float radius, float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawText(std::string text, float coords[2], float color[4], float scale, std::string align) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  3D API.																																	   //
//=============================================================================================================================================//

 void EngineCoreGL::drawQuadFilled3D(float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float color[4]) { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawCuboidFilled(float vertex1[3], float vertex2[3], float vertex3[3], float vertex4[3], float depth, float color[4]) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  Viewport.																																   //
//=============================================================================================================================================//

 void EngineCoreGL::resizeEvent(float with, float height) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//