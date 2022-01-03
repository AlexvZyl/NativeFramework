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
	m_camera				= std::make_unique<Camera>(cameraType);
	// VAO's.
	m_linesVAO				= std::make_unique<VertexArrayObject<VertexData>>(GL_LINES);
	m_trianglesVAO			= std::make_unique<VertexArrayObject<VertexData>>(GL_TRIANGLES);
	m_texturedTrianglesVAO  = std::make_unique<VertexArrayObject<VertexDataTextured>>(GL_TRIANGLES);
	m_circlesVAO			= std::make_unique<VertexArrayObject<VertexDataCircle>>(GL_TRIANGLES);
}

//==============================================================================================================================================//
//  Methods.																																	//
//==============================================================================================================================================//

glm::mat4* Scene::getViewMatrix() { return &m_camera->m_viewMatrix; }

void Scene::updateCamera() { m_camera->updateCamera(); }

glm::vec2* Scene::getViewportDimensions() { return &m_FBO->m_viewport; }

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//