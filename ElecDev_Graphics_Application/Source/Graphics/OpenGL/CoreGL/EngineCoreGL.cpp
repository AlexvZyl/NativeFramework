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
#include "VertexArrayObjectGL.h"
#include "Resources/ResourceHandler.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "GLFW/glfw3.h"
#include "ShaderHandlerGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "glm/gtc/matrix_transform.hpp"
#include "CoreGL/Entities/Vertex.h"
//#include "Fonts.h"
#include "FontLoader.h"
#include <memory>

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

// Constructor.
EngineCoreGL::EngineCoreGL(GUIState* guiState, std::string contextName)
	:m_guiState(guiState), m_contextName(contextName)
{
	// Print start message.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core starting...";

	createDefaultBackground();

	// ----------------------------------------- //
	//  C R E A T E   T E X T   R E N D E R E R  //
	// ----------------------------------------- //

	// Load font from resource.
	m_defaultFont = std::make_unique<Font>(msdfLoadFont(ARIAL_NORMAL_JSON, ARIAL_NORMAL_PNG));
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1, 2 };		// 0 = No texture, 1 = Font atlas, 2+ = Any other textures.
	GLCall(glUniform1iv(loc, 3, samplers));
	GLCall(glBindTextureUnit(1, m_defaultFont->textureID));	// Text Atlas.

	// Print done message.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core done.\n";
}

// Destructor.
EngineCoreGL::~EngineCoreGL() {}

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
	float viewport[2] = { m_guiState->renderWindowSize[0], m_guiState->renderWindowSize[1] };
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
	// Primitives.
	m_linesVAO->resize();
	m_trianglesVAO->resize();
	m_texturedTrianglesVAO->resize();
	m_circlesVAO->resize();
}

void EngineCoreGL::createDefaultBackground()
{
	// Background vertices used to render the gradient (Still need to implement).

	//			1 ----- 2 ----- 3 ----- 4
	//			|	    |	    |	    |
	//			|	    |       |	    |
	//			5 ----- 6 ----- 7 ----- 8
	//			|	    |	    |	    |
	//			|       |	    |	    |
	//			9 ----- 10 ---- 11 ---- 12
	//			|	    |	    |	    |
	//			|	    |	    |	    |
	//			13 ---- 14 ---- 15 ---- 16

	// Create the VAO.
	m_backgroundVAO = std::make_unique<VertexArrayObject<VertexData>>(GL_TRIANGLES);
	// Assign background data.
	glm::vec4 bgColor1((float)182 / 255, (float)200 / 255, (float)255 / 255, 1.f);
	glm::vec4 bgColor2((float)222 / 255, (float)255 / 255, (float)255 / 255, 1.f);
	/*glm::vec4 bgColor1((float)35 / 255, (float)35 / 255, (float)40 / 255, 1.f);
	glm::vec4 bgColor2((float)35 / 255, (float)35 / 255, (float)40 / 255, 1.f);*/
	glm::vec3 pos1(1.0f, 1.0f, 0.99f);
	glm::vec3 pos2(-1.0f, 1.0f, 0.99);
	glm::vec3 pos3(-1.0f, -1.0f, 0.99);
	glm::vec3 pos4(1.0f, -1.0f, 0.99);
	// Create and append the vertices.
	std::vector<VertexData> vertices =
	{
		VertexData(pos1, bgColor2, -1),	//  Top right.
		VertexData(pos2, bgColor1, -1),	//  Top left.
		VertexData(pos3, bgColor1, -1),	//  Bottom left.
		VertexData(pos4, bgColor1, -1)	//  Bottom right.
	};
	// Create background.
	std::vector<unsigned> indices({ 0,1,2,2,3,0 });
	m_backgroundVAO->appendVertexData(vertices, indices);
	m_backgroundVAO->resize();
}

float EngineCoreGL::deltaTime()
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	return m_deltaTime;
}

//=============================================================================================================================================//
//  User input.																																//
//=============================================================================================================================================//

void EngineCoreGL::mouseScrollEvent(float pixelCoords[2], float yOffset) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressLeft(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressRight(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mousePressMiddle(float pixelCoords[2]) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::mouseMoveEvent(float pixelCoords[2], int buttonStateLeft, int buttonStateRight, int buttonStateMiddle) { functionNotImplementedError(__FUNCTION__); }
void EngineCoreGL::keyEvent(int key, int action) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  Viewport.																																   //
//=============================================================================================================================================//

void EngineCoreGL::resizeEvent(float with, float height) { functionNotImplementedError(__FUNCTION__); }

//=============================================================================================================================================//
//  EOF.																																	   //
//=============================================================================================================================================//