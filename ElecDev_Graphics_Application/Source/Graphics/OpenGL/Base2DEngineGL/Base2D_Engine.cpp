//=============================================================================================================================================//
//  Includes.																																   //
//=============================================================================================================================================//

// Needed to load resources.
#include "../../Resources/ResourceHandler.h"
// Class include.
#include "Base2D_Engine.h"
#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/ShaderHandlerGL.h"
#include <iostream>
#include "Misc/ConsoleColor.h"
#include "glm/gtc/matrix_transform.hpp"
#include "CoreGL/Fonts.h"

//=============================================================================================================================================//
//  Costructor & Destructor.																												   //
//=============================================================================================================================================//

// With GLFW window.
Base2DEngineGL::Base2DEngineGL(GUIState* guiState, std::string contextName) 
	: EngineCoreGL(guiState, contextName)
{
	std::cout << blue << "[OPENGL] [INFO] : " << white << "Base 2D engine starting...";

	// Create the background shader.
	m_backgroundShader = std::make_unique<Shader>(BACKGROUND_SHADER_2D);

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
	m_projectionValues[4] = -1.0f;
	m_projectionValues[5] = 1.0f;
	m_projectionMatrix = glm::ortho(m_projectionValues[0], m_projectionValues[1], m_projectionValues[2], m_projectionValues[3], m_projectionValues[4], m_projectionValues[5]);

	// Assign projection matrices to shader.
	m_basicShader->bind();
	m_basicShader->setMat4("projectionMatrix", &m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", &m_projectionMatrix);

	// ------------------------------- //
	//  R E N D E R I N G   S E T U P  //
	// ------------------------------- //

	// Add a texture to the texture shader.
	m_texture = loadBitmapToGL(loadImageFromResource(CIRCUIT_TREE_PNG));
	m_textureShader->bind();
	GLCall(auto loc = glGetUniformLocation(m_textureShader->m_rendererID, "f_textures"));
	int samplers[4] = { 0, 1, 2, 3 };
	GLCall(glUniform1iv(loc, 4, samplers));
	GLCall(glBindTextureUnit(3, m_texture));

	//---------------------------------------------------------------------------------------

	// Done.
	std::cout << blue << "\n[OPENGL] [INFO] : " << white << "Base 2D engine done.";

};

// Delete and free memory.
Base2DEngineGL::~Base2DEngineGL() {}

//=============================================================================================================================================//
//  Rendering.																																   //
//=============================================================================================================================================//

// [MAIN LOOP] The rendering pipeline.
void Base2DEngineGL::renderLoop()
{

}

// Return the ID to the texture that is rendered via the FBO.
unsigned int Base2DEngineGL::getRenderTexture() { return m_frameBuffer->getRenderTexture(); }

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
	m_basicShader->setMat4("projectionMatrix", &m_projectionMatrix);
	m_textureShader->bind();
	m_textureShader->setMat4("projectionMatrix", &m_projectionMatrix);
	m_circleShader->bind();
	m_circleShader->setMat4("projectionMatrix", &m_projectionMatrix);

	// Resize FBO attachments.
	m_frameBuffer->resize(width, height);

	// Change viewport dimmensions.
	m_imGuiViewportDimensions[0] = (float)width;
	m_imGuiViewportDimensions[1] = (float)height;
}

//=============================================================================================================================================//
// EOF.																																		   //
//=============================================================================================================================================//