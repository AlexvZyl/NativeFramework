//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Buffers/FrameBufferObjectGL.h"
#include "OpenGL/Buffers/VertexArrayObjectGL.h"
#include "OpenGL/Primitives/Vertex.h"
#include "OpenGL/SceneGL.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Entities/Entity.h"
#include "OpenGL/Primitives/TextureGL.h"
#include "OpenGL/Primitives/Primitive.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Application/Events/Events.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "imgui/imgui.h"
#include "OpenGL/ErrorHandlerGL.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, float width, float height) 
{
	// FBO.
	m_FBO					= std::make_unique<FrameBufferObject>(width, height);
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

Scene::~Scene() 
{
	m_primitives.clear();
	Renderer::doneSceneDestruction();
}

//==============================================================================================================================================//
//  Camera Methods.																																//
//==============================================================================================================================================//

glm::mat4* Scene::getViewMatrix() 
{ 
	return &m_camera->m_viewMatrix; 
}

glm::mat4* Scene::getProjectionMatrix() 
{ 
	return &m_camera->m_projectionMatrix; 
}

void Scene::onUpdate() 
{ 
	m_camera->onUpdate(); 
}

glm::vec4& Scene::getViewport() 
{ 
	return m_camera->m_viewportVec;
}

void Scene::setViewport(int width, int height) 
{ 
	m_camera->setViewport(width, height); 
}

//==============================================================================================================================================//
//  FBO Methods.																															    //	
//==============================================================================================================================================//

void Scene::onRenderInit() 
{
	m_FBO->bind();
	m_FBO->clear();
	onUpdate();
	Renderer::setViewport(getViewport());
	m_FBO->setDrawBuffers();
}

void Scene::onRenderCleanup() 
{
	m_FBO->unbind();
}

unsigned Scene::getRenderTexture() 
{ 
	return m_FBO->getRenderTexture(); 
}

unsigned Scene::getEntityID(const glm::vec2& pixelCoords)
{
	// Adjust the pixel coords.
	return m_FBO->getEntityID({pixelCoords.x, m_camera->m_viewportVec[3] - pixelCoords.y});
}

void Scene::deleteGPUResources() 
{
	m_FBO->deleteResources();
}

void Scene::recreateGPUResources() 
{
	m_FBO->createResources(m_camera->m_viewportVec[2], m_camera->m_viewportVec[3]);
}

//==============================================================================================================================================//
//  Coordinates.																															    //
//==============================================================================================================================================//

glm::vec3 Scene::pixelCoordsToWorldCoords(const glm::vec2& pixelCoords)
{
	return m_camera->pixelCoordsToWorldCoords(pixelCoords);
}

glm::vec3 Scene::pixelCoordsToCameraCoords(const glm::vec2& pixelCoords) 
{ 
	return m_camera->pixelCoordsToCameraCoords(pixelCoords); 
}

//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (m_camera->m_type == CameraType::Standard2D) create2DBackground();
	else if (m_camera->m_type == CameraType::Standard3D) create2DBackground();
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
	m_backgroundVAO->setBufferIncrementSize(5);

	// Assign background data.
	glm::vec4 bgColor2((float)217 / 255, (float)250 / 255, (float)255 / 255, 1.f);
	glm::vec4 bgColor1((float)182 / 255, (float)200 / 255, (float)255 / 255, 1.f);
	//glm::vec4 defaultCol((float)92 / 255, (float)95 / 255, (float)103 / 255, 1.f);
	glm::vec4 defaultCol((float)66 / 255, (float)66 / 255, (float)68 / 255, 1.f);
	glm::vec3 pos1(1.0f, 1.0f, 0.f);
	glm::vec3 pos2(-1.0f, 1.0f, 0.f);
	glm::vec3 pos3(-1.0f, -1.0f, 0.f);
	glm::vec3 pos4(1.0f, -1.0f, 0.f);

	// Vertices.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos1, defaultCol, -1)); //  Top right.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos2, defaultCol, -1)); //  Top left.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos3, defaultCol, -1)); //  Bottom left.
	m_backgroundVAO->m_vertexCPU.emplace_back(VertexData(pos4, defaultCol, -1)); //  Bottom right.
	m_backgroundVAO->m_vertexCount += 4;

	// Indices.
	m_backgroundVAO->m_indexCPU.insert(m_backgroundVAO->m_indexCPU.end(), { 0,1,2, 2,3,0 });
	m_backgroundVAO->m_indexCount += 6;

	// Data will be set on the first resize.
	m_backgroundVAO->queryBufferResize();
}

void Scene::create3DBackground() 
{
	// Implement skybox.
}

//==============================================================================================================================================//
//  Events.																																		//
//==============================================================================================================================================//

void Scene::resize(const glm::vec2& size) 
{
	m_camera->resize((int)size.x, (int)size.y);
	m_FBO->resize((int)size.x, (int)size.y);
}

void Scene::onEvent(Event& event) 
{
	if (event.isConsumed()) return;

}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//