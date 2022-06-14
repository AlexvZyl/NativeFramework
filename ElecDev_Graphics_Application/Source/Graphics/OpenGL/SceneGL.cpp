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
#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/ErrorHandlerGL.h"
#include "OpenGL/Primitives/Grid.h"
#include "Application/Events/Events.h"
#include "imgui/imgui.h"

//==============================================================================================================================================//
//  Constructor & Destructor.																													//
//==============================================================================================================================================//

Scene::Scene(CameraType cameraType, const glm::vec2& size) 
{
	// FBO.
	m_FBO					= std::make_unique<FrameBufferObject>((int)size.x, (int)size.y);

	// Camera.
	m_camera				= std::make_unique<Camera>(cameraType, size);

	// VAO's.
	m_linesBuffer				= std::make_unique<GraphicsLinesBuffer<VertexData>>();
	m_trianglesBuffer			= std::make_unique<GraphicsTrianglesBuffer<VertexData>>();
	m_texturedTrianglesBuffer	= std::make_unique<GraphicsTrianglesBuffer<VertexDataTextured>>();
	m_circlesBuffer				= std::make_unique<GraphicsTrianglesBuffer<VertexDataCircle>>();

	// Grid.
	Renderer::storeAndBindScene(this);
	m_grid = std::make_unique<Grid>();
	Renderer::restoreAndUnbindScene();

	// Background.
	// Currently not in use.
	createDefaultBackground();
}

Scene::~Scene() 
{
	m_grid.reset();
	m_primitives.clear();
	Renderer::doneSceneDestruction();
}

Camera& Scene::getCamera()
{
	return *m_camera.get();
}

Grid& Scene::getGrid() 
{
	return *m_grid.get();
}

//==============================================================================================================================================//
//  FBO Methods.																															    //	
//==============================================================================================================================================//

void Scene::onRenderInit() 
{
	m_FBO->bind();
	m_FBO->clear();
	getCamera().onUpdate();
	Renderer::setViewport(getCamera().getViewport());
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
	return m_FBO->getEntityID(pixelCoords);
}

void Scene::deleteGPUResources() 
{
	m_FBO->deleteResources();
}

void Scene::recreateGPUResources() 
{
	m_FBO->createResources(getCamera().getViewport()[2], getCamera().getViewport()[3]);
}

//==============================================================================================================================================//
//  Backgrounds.																																	//
//==============================================================================================================================================//

void Scene::createDefaultBackground() 
{
	if      (getCamera().getType() == CameraType::Standard2D) create2DBackground();
	else if (getCamera().getType() == CameraType::Standard3D) create2DBackground();
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
	m_backgroundBuffer = std::make_unique<GraphicsTrianglesBuffer<VertexData>>();
	m_backgroundBuffer->setCapacityIncrements(2);

	// Vertices.
	VertexData vertices[4] = {
		{ { 1.0f, 1.0f, 0.f   }, Renderer::backgroundColor, (unsigned) -1 }, //  Top right.
		{ { -1.0f, 1.0f, 0.f  }, Renderer::backgroundColor, (unsigned) -1 }, //  Top left.
		{ { -1.0f, -1.0f, 0.f }, Renderer::backgroundColor, (unsigned) -1 }, //  Bottom left.
		{ { 1.0f, -1.0f, 0.f  }, Renderer::backgroundColor, (unsigned) -1 }  //  Bottom right.
	};

	// Indices.
	const UInt3 indices[2] = {
		{ 0, 1, 2 },
		{ 2, 3, 0 }
	};

	// Push data.
	m_backgroundBuffer->push(vertices, 4, indices, 2);
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
	getCamera().resize(size);
	m_FBO->resize((int)size.x, (int)size.y);
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//