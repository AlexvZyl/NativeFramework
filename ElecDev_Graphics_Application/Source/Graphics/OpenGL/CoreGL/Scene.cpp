//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "CoreGL/FrameBufferObjectGL.h"
#include "CoreGL/VertexArrayObjectGL.h"
#include "CoreGL/Entities/Vertex.h"
#include "CoreGL/Scene.h"
#include "CoreGL/Camera.h"
#include "CoreGL/Entities/Entity.h"
#include "CoreGL/Texture.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, float width, float height, unsigned msaaSamples) 
{
	// FBO.
	m_FBO					= std::make_unique<FrameBufferObject>(width, height, msaaSamples);
	// Camera.
	m_camera				= std::make_unique<Camera>(cameraType, width, height);
	// VAO's.
	m_linesVAO				= std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	m_trianglesVAO			= std::make_unique<VertexArrayObject<VertexData>>(GL_TRIANGLES);
	m_texturedTrianglesVAO  = std::make_unique<VertexArrayObject<VertexDataTextured>>(GL_TRIANGLES);
	m_circlesVAO			= std::make_unique<VertexArrayObject<VertexDataCircle>>(GL_TRIANGLES);
	// Background.
	createDefaultBackground();
}

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

glm::mat4* Scene::getViewMatrix() { return &m_camera->m_viewMatrix; }

glm::mat4* Scene::getProjectionMatrix() { return &m_camera->m_projectionMatrix; }

void Scene::updateCamera() { m_camera->updateCamera(); }

glm::vec2 Scene::getViewport() { return glm::vec2(m_camera->m_viewportVec.x, m_camera->m_viewportVec.y); }

void Scene::setViewport(int width, int height) { m_camera->setViewport(width, height); }

unsigned Scene::getRenderTexture() { return m_FBO->getRenderTexture(); }

glm::vec3 Scene::pixelCoordsToWorldCoords(float pixelCoords[2])  { return m_camera->pixelCoordsToWorldCoords(pixelCoords);  }

glm::vec3 Scene::pixelCoordsToCameraCoords(float pixelCoords[2]) { return m_camera->pixelCoordsToCameraCoords(pixelCoords); }

unsigned Scene::getEntityID(glm::vec2& pixelCoords) 
{
	// Adjust the pixel coords.
	glm::vec2 pixelCoordsTemp = { pixelCoords[0], m_camera->m_viewportVec[3] - pixelCoords[1]};
	return m_FBO->getEntityID(pixelCoordsTemp);
}


//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (m_camera->m_type == CameraType::Standard2D) create2DBackground();
	else if (m_camera->m_type == CameraType::Standard3D) create2DBackground();  // TODO: Implement skybox.
}

void Scene::create2DBackground()
{
	// TODO: Background vertices used to render the gradient.
	// 
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
}

void Scene::create3DBackground() 
{
	
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void Scene::resize(int width, int height) 
{
	m_camera->setViewport(width, height);
	m_FBO->resize(width, height);

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

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//