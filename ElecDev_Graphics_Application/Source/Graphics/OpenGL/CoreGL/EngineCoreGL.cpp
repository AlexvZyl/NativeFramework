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

//=============================================================================================================================================//
//  Constructor & Destructor.																												   //
//=============================================================================================================================================//

EngineCoreGL::EngineCoreGL(stateMachine* states)
	:m_states(states)
{
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Engine core starting...";

	//---------------------------------------------------------------------------------------
	// Compile basic shaders.
	//---------------------------------------------------------------------------------------

	m_basicShader = new Shader(BASIC_SHADER);
	m_backgroundShader = new Shader(BACKGROUND_SHADER);
	m_textureShader = new Shader(TEXTURE_SHADER);

	// Set default values for the shaders.
	glm::mat4 identity = glm::mat4(1.0f);
	m_basicShader->bind();
	m_basicShader->setMat4("worldMatrix", identity);
	m_basicShader->setMat4("projectionMatrix", identity);
	m_basicShader->setMat4("viewMatrix", identity);
	m_textureShader->bind();
	m_textureShader->setMat4("worldMatrix", identity);
	m_textureShader->setMat4("projectionMatrix", identity);
	m_textureShader->setMat4("viewMatrix", identity);

	//---------------------------------------------------------------------------------------
	// Create basic VAO's.
	//---------------------------------------------------------------------------------------

	// Buffers setup.
	m_linesVAO = new VertexArrayObject(GL_LINES);
	m_trianglesVAO = new VertexArrayObject(GL_TRIANGLES);
	m_texturedTrianglesVAO = new VertexArrayObject(GL_TRIANGLES, true);
	m_frameBuffer = new FrameBufferObject((int)m_imGuiViewportDimensions[0], (int)m_imGuiViewportDimensions[1], 8);
	m_backgroundVAO = new VertexArrayObject(GL_TRIANGLES);

	//---------------------------------------------------------------------------------------
	// Create default text renderer.
	//---------------------------------------------------------------------------------------

	m_textRenderer = new TextRenderer(ARIAL_SDF_FNT, ARIAL_SDF_PNG);
	// Setup shader with textures for text renderer.
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[3] = { 0, 1 };
	GLCall(glUniform1iv(loc, 2, samplers));
	GLCall(glBindTextureUnit(1, m_textRenderer->m_textureID));	// Text Atlas.

	//---------------------------------------------------------------------------------------

	createDefaultBackground();

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Engine core done.\n";
}

EngineCoreGL::~EngineCoreGL()
{
	// Shaders.
	delete m_basicShader;
	delete m_backgroundShader;
	delete m_textureShader;
	// VAO's.
	delete m_linesVAO;
	delete m_backgroundVAO;
	delete m_trianglesVAO;
	delete m_texturedTrianglesVAO;
	// Text renderer.
	delete m_textRenderer;
	// FBO.
	delete m_frameBuffer;
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

void EngineCoreGL::updateGPU() 
{
	m_linesVAO->updateGPU();
	m_trianglesVAO->updateGPU();
	m_texturedTrianglesVAO->updateGPU();
}
												
 void EngineCoreGL::autoCenter() { functionNotImplementedError(__FUNCTION__); }
 void EngineCoreGL::drawDemo(unsigned int loopCount) { functionNotImplementedError(__FUNCTION__); }
												
 unsigned int EngineCoreGL::getRenderTexture() 
 {
	 return m_frameBuffer->getRenderTexture();
 }

 void EngineCoreGL::createDefaultBackground() 
 {
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