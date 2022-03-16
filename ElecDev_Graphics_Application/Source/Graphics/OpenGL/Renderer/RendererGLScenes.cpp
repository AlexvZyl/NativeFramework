//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "OpenGL/Renderer/RendererGL.h"
#include "OpenGL/SceneGL.h"
#include "OpenGL/ShaderGL.h"
#include "Lumen.h"
#include "Graphics/Camera/Camera.h"
#include "Utilities/Profiler/Profiler.h"
#include "Application/Application.h"

//==============================================================================================================================================//
//  General.																																	//
//==============================================================================================================================================//

void Renderer::bindScene(Scene* scene)
{
	m_scene = scene;
}

void Renderer::unbindScene()
{
	m_scene = nullptr;
}

Scene* Renderer::getScene()
{
	return m_scene;
}

void Renderer::initSceneDestruction(Scene* scene)
{
	// Store current scene.
	m_storedScene = m_scene;
	// Bind scene to be destroyed.
	m_scene = scene;
}

void Renderer::doneSceneDestruction()
{
	// Restore scene.
	if (m_scene != m_storedScene) { m_scene = m_storedScene; }
	else						  { m_scene = nullptr;		 }
	// Remove stored scene.
	m_storedScene = nullptr;
}

//==============================================================================================================================================//
//  Rendering.																																	//
//==============================================================================================================================================//

void Renderer::renderScene()
{
	Renderer::renderScene(m_scene);
}

void Renderer::renderScene(Scene* scene)
{
	LUMEN_PROFILE_SCOPE("Render Scene");

	// 2D Pipeline.
	if (scene->m_camera->m_type == CameraType::Standard2D)	
	{ 
		Renderer::geometryPass2D(scene); 
	}

	// 3D Pipeline.
	else if (scene->m_camera->m_type == CameraType::Standard3D) 
	{ 
		Renderer::geometryPass3D(scene);
	}

	// Nothing to render.
	else return;

	//  Resolve the MSAA.
	scene->m_FBO->renderFromMSAA();
}

void Renderer::geometryPass2D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_BLEND);
	Renderer::setViewport(scene->getViewport());
	scene->onUpdate();
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw background.
	m_shaders["BackgroundShader"]->bind();
	Renderer::setDepthFunc(GL_EQUAL);
	Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());
	Renderer::setDepthFunc(GL_LESS);

	// Draw basic primitives.
	shader = m_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = m_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = m_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	scene->unbindFBO();
	Renderer::disable(GL_BLEND);
}

void Renderer::geometryPass3D(Scene* scene)
{
	LUMEN_RENDER_PASS();

	// ----------- //
	//  S E T U P  //
	// ----------- //

	Renderer::enable(GL_BLEND);
	Renderer::setViewport(scene->getViewport());
	scene->onUpdate();
	scene->bindFBO();
	scene->clearFBO();

	// ------------------- //
	//  R E N D E R I N G  //
	// ------------------- //

	// The shader used in rendering.
	Shader* shader = nullptr;

	// Draw background.
	m_shaders["BackgroundShader"]->bind();
	Renderer::setDepthFunc(GL_EQUAL);
	Renderer::drawBufferIndexed(scene->m_backgroundVAO.get());
	Renderer::setDepthFunc(GL_LESS);

	// Draw basic primitives.
	shader = m_shaders["BasicShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_linesVAO.get());
	Renderer::drawBufferIndexed(scene->m_trianglesVAO.get());

	// Draw textured primitives.
	shader = m_shaders["TextureShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::loadTextures(scene);
	Renderer::drawBufferIndexed(scene->m_texturedTrianglesVAO.get());

	// Draw Circles.
	shader = m_shaders["CircleShader"].get();
	shader->bind();
	shader->setMat4("viewMatrix", scene->getViewMatrix());
	shader->setMat4("projectionMatrix", scene->getProjectionMatrix());
	Renderer::drawBufferIndexed(scene->m_circlesVAO.get());

	// --------------- //
	//  C L E A N U P  //
	// --------------- //

	scene->unbindFBO();
	Renderer::disable(GL_BLEND);
}

//==============================================================================================================================================//
//  Default Scenes.																																//
//==============================================================================================================================================//

void Renderer::generateDefaultScenes()
{
	m_default2DScene = std::make_unique<Scene>(CameraType::Standard2D, 900, 900, 1);
	// Render the scene once so that the background can be generated.
	Renderer::renderScene(m_default2DScene.get());
	m_default2DScene->m_FBO->renderFromMSAA();
}

unsigned Renderer::getDefault2DSceneTexture()
{
	return m_default2DScene->getRenderTexture();
}

//==============================================================================================================================================//
//  EOF.																																		//
//==============================================================================================================================================//